#include "src/.h/scenegl.h"

SceneGL::SceneGL(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    /* [0]:modulo( r ),
     * [1]:angle( theta ),
     * [2]:angle( phi )
        unit of angle : rad*/
//    target_pos_[0] = 0.0; target_pos_[1] = 0.0; target_pos_[2] = 0.0;
//    cam_pos_vec_[0] = 100.0;cam_pos_vec_[1] = M_PI / 3;cam_pos_vec_[2] = M_PI * 3 / 2;
    target_pos_[0] = 20.1493; target_pos_[1] = 0.733772; target_pos_[2] = 1.88334;
    cam_pos_vec_[0] = 159.0;cam_pos_vec_[1] = 1.0015;cam_pos_vec_[2] = 4.35825;

    catapult_status_ = NULL;
    round = NULL;
    firing_ = false;
}

SceneGL::~SceneGL()
{
    if(catapult_status_ != NULL)
        delete catapult_status_;

    glDeleteTextures (1, &TARGET_texture_);
    glDeleteTextures (1, &LOGO_texture_);


}

void SceneGL::setCatapultAngle(float h, float v)
{
    if(catapult_status_ !=NULL)
    {
        catapult_status_->setAngleH(h);
        catapult_status_->setAngleTrebuchet(v);
    }
}

void SceneGL::drawAnime()
{
    firing_ = true;
//    cameraBackup();

    double i = 1.0;
    while(!catapult_status_->AngleTrebuchetReady())
    {
        catapult_status_->setAngleTrebuchet((double)(catapult_status_->getAngleTrebuchet() - 0.4 * i));
        if(catapult_status_->getAngleTrebuchet() < 0)
        {
            i+=0.1;
        }
        updateGL();
    }
    catapult_status_->setHSpeed();
    i = 1;
    while(!catapult_status_->SphereOutofBounds())
    {
        updateGL();
        catapult_status_->SphereZDecrement(0.1 + 0.001 * (i++));
    }
    round->calculScore(qSqrt(qPow((catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8)) * qSin(-catapult_status_->getAngleH() / 180.0 * M_PI) -
                                  round->getPositionX(), 2) +
                             qPow((catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8)) * qCos(catapult_status_->getAngleH() / 180.0 * M_PI) + 5 -
                                  round->getPositionY(), 2)));
//    qDebug()<<"1"<<(catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8)) * qSin(-catapult_status_->getAngleH() / 180.0 * M_PI);
//    qDebug()<<"2"<<round->getPositionX();
//    qDebug()<<"3"<<(catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8)) * qCos(catapult_status_->getAngleH() / 180.0 * M_PI) + 5;
//    qDebug()<<"4"<<round->getPositionY();

//    firing_ = false;
//    cameraRecover();
//    updateGL();
}

//void SceneGL::cameraBackup()
//{
//    target_pos_backup_[0] = target_pos_[0]; target_pos_backup_[1] = target_pos_[1]; target_pos_backup_[2] = target_pos_[2];
//    cam_pos_vec_backup_[0] = cam_pos_vec_[0]; cam_pos_vec_backup_[1] = cam_pos_vec_[1]; cam_pos_vec_backup_[2] = cam_pos_vec_[2];
//}

//void SceneGL::cameraRecover()
//{
//    target_pos_[0] = target_pos_backup_[0]; target_pos_[1] = target_pos_backup_[1]; target_pos_[2] = target_pos_backup_[2];
//    cam_pos_vec_[0] = cam_pos_vec_backup_[0]; cam_pos_vec_[1] = cam_pos_vec_backup_[1]; cam_pos_vec_[1] = cam_pos_vec_backup_[1];
//}

static void NormalizeAngle(double &angle)
{
    if(angle < 0)
    {
        angle += 2 * M_PI;
    }
    else if(angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }
}

void SceneGL::initializeGL()
{
    qglClearColor(Qt::black);

//    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
//    glEnable(GL_TEXTURE_2D);

//    GLfloat global_light_position[] = { 0.0f ,0.0f ,1.0f ,0.0f };
    GLfloat global_light_ambient[] = { 1.0f ,1.0f ,1.0f ,1.0f };
//    GLfloat global_light_diffuse[] = { 1.0f ,1.0f ,1.0f ,1.0f };

//    glLightfv(GL_LIGHT0, GL_POSITION, global_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, global_light_ambient);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
//    glDisable(GL_LIGHTING);

//    loadTextures_logo();
    initGlobalSceneList();
    initCatapult();

}

void SceneGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    move_camera();
    draw();
}

void SceneGL::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (double)width/height, .1, 400);

    glMatrixMode(GL_MODELVIEW);
}

void SceneGL::mousePressEvent(QMouseEvent *event)
{
    lastPos_ = event->pos();
}

void SceneGL::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() & Qt::LeftButton)
    {
        rotate_camera(event->x(), event->y());
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        translate_camera(event->x(), event->y());
        updateGL();
    }

    lastPos_ = event->pos();
}

void SceneGL::wheelEvent(QWheelEvent *event)
{
    cam_pos_vec_[0] -= event->delta()/120;
    if(cam_pos_vec_[0] < 0.1)
    {
        cam_pos_vec_[0] = 0.1;
    }
    else if(cam_pos_vec_[0] > 200)
    {
        cam_pos_vec_[0] = 200;
    }
    updateGL();
}

void SceneGL::draw()
{
    glCallList(global_scene_list_);
    glPushMatrix();
        glTranslatef(0, 5, 0);
        glRotatef(catapult_status_->getAngleH(), 0, 0, 1);
        glCallList(catapult_base_);
        glPushMatrix();
            glTranslatef(0, 0, 8);
            glPushMatrix();
                glPushMatrix();
                    if(!catapult_status_->AngleTrebuchetReady())
                    {
                        if(catapult_status_->getAngleTrebuchet() >= 0)
                        {
                            glTranslatef(0, catapult_status_->getSphereYPos(), -7.5);
                        }
                        else
                        {
                            glTranslatef(0, catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI), catapult_status_->getTrebuchetBottomZPos() - qCos(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5);
                        }
                    }
                    else
                    {
                        glTranslatef(0, catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8), catapult_status_->getSphereZ() - 8);
                    }
                    drawSphere();
                glPopMatrix();
                drawConnecter();
                glRotatef(catapult_status_->getAngleTrebuchet(), 1, 0, 0);
                glCallList(trebuchet_);
            glPopMatrix();
            glTranslatef(0, 5 * qCos(catapult_status_->getAngleTrebuchet() / 180.0 * M_PI), 5 * qSin(catapult_status_->getAngleTrebuchet() / 180.0 * M_PI));
            glCallList(trebuchet_load_);
        glPopMatrix();
    glPopMatrix();
    if(round!=NULL){
        draw_target();
    }
}

void SceneGL::move_camera()
{
    double vec_up_theta = cam_pos_vec_[1] - M_PI / 2;
    if(vec_up_theta < 0)
    {
        vec_up_theta += 2 * M_PI;
    }
    if(firing_)
    {
        double cam_center[3];
        if(!catapult_status_->AngleTrebuchetReady())
        {
            if(catapult_status_->getAngleTrebuchet() >= 0)
            {
                cam_center[0] = catapult_status_->getSphereYPos() * qSin(-catapult_status_->getAngleH() / 180.0 * M_PI);
                cam_center[1] = catapult_status_->getSphereYPos() * qCos(catapult_status_->getAngleH() / 180.0 * M_PI) + 5;
                cam_center[2] = 0;
            }
            else
            {
                cam_center[0] = (catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI)) * qSin(-catapult_status_->getAngleH() / 180.0 * M_PI);
                cam_center[1] = (catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI)) * qCos(catapult_status_->getAngleH() / 180.0 * M_PI) + 5;
                cam_center[2] = catapult_status_->getTrebuchetBottomZPos() - qCos(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5 + 7.5;
//                glTranslatef(0, catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI), catapult_status_->getTrebuchetBottomZPos() - qCos(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5);
            }
        }
        else
        {
            cam_center[0] = catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8) * qSin(-catapult_status_->getAngleH() / 180.0 * M_PI);
            cam_center[1] = catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8) * qCos(catapult_status_->getAngleH() / 180.0 * M_PI) + 5;
            cam_center[2] = catapult_status_->getSphereZ() - 0.5;
//            glTranslatef(0, catapult_status_->getHSpeed() * qSqrt(2 * (32.5 - catapult_status_ -> getSphereZ()) / 9.8), catapult_status_->getSphereZ() - 8);
        }
        gluLookAt(cam_center[0] + cam_pos_vec_[0] * qSin(cam_pos_vec_[1]) * qCos(cam_pos_vec_[2]),
                  cam_center[1] + cam_pos_vec_[0] * qSin(cam_pos_vec_[1]) * qSin(cam_pos_vec_[2]),
                  cam_center[2] + cam_pos_vec_[0] * qCos(cam_pos_vec_[1]),

                  cam_center[0], cam_center[1], cam_center[2],

                  1.0 * qSin(vec_up_theta) * qCos(cam_pos_vec_[2]),
                  1.0 * qSin(vec_up_theta) * qSin(cam_pos_vec_[2]),
                  1.0 * qCos(vec_up_theta));
    }
    else
    {
        gluLookAt(target_pos_[0] * qSin(target_pos_[1]) * qCos(target_pos_[2]) +
                cam_pos_vec_[0] * qSin(cam_pos_vec_[1]) * qCos(cam_pos_vec_[2]),
                target_pos_[0] * qSin(target_pos_[1]) * qSin(target_pos_[2]) +
                cam_pos_vec_[0] * qSin(cam_pos_vec_[1]) * qSin(cam_pos_vec_[2]),
                target_pos_[0] * qCos(target_pos_[1]) +
                cam_pos_vec_[0] * qCos(cam_pos_vec_[1]),

                target_pos_[0] * qSin(target_pos_[1]) * qCos(target_pos_[2]),
                target_pos_[0] * qSin(target_pos_[1]) * qSin(target_pos_[2]),
                target_pos_[0] * qCos(target_pos_[1]),

                1.0 * qSin(vec_up_theta) * qCos(cam_pos_vec_[2]),
                1.0 * qSin(vec_up_theta) * qSin(cam_pos_vec_[2]),
                1.0 * qCos(vec_up_theta));
    }
//    qDebug()<<"camera center:"<<target_pos_[0]<<target_pos_[1]<<target_pos_[2];
//    qDebug()<<"camera vector:"<<cam_pos_vec_[0]<<cam_pos_vec_[1]<<cam_pos_vec_[2];
}

void SceneGL::rotate_camera(double X, double Y)
{
    double dtheta = ((double)(lastPos_.y() - Y) / (double) width()) * M_PI;
    double dphi = ((double)(lastPos_.x() - X) / (double) width()) * M_PI;

    cam_pos_vec_[1] += dtheta;
    NormalizeAngle(cam_pos_vec_[1]);
    if(cam_pos_vec_[1] > 0 && cam_pos_vec_[1] < M_PI)
    {
        cam_pos_vec_[2] += dphi;
    }
    else
    {
        cam_pos_vec_[2] -= dphi;
    }
    NormalizeAngle(cam_pos_vec_[2]);
}

void SceneGL::translate_camera(double X, double Y)
{
    double cam_theta_vec = cam_pos_vec_[1] - M_PI/2;
    double cam_phi_vec = cam_pos_vec_[2] + M_PI/2;
    if(cam_theta_vec < 0)
    {
        cam_theta_vec += 2 * M_PI;
    }
    if(cam_phi_vec > 2 * M_PI)
    {
        cam_phi_vec -= 2 * M_PI;
    }
    double dV = 0 - ((double)(lastPos_.y() - Y) / (double) width());
    double dH = ((double)(lastPos_.x() - X) / (double) width());

    dV *=10;dH *= 10;

    double x = target_pos_[0] * qSin(target_pos_[1]) * qCos(target_pos_[2]) +
               1.0 * qSin(cam_theta_vec) * qCos(cam_pos_vec_[2]) * dV +
               1.0 * qSin(M_PI/2) * qCos(cam_phi_vec) * dH;
    double y = target_pos_[0] * qSin(target_pos_[1]) * qSin(target_pos_[2]) +
               1.0 * qSin(cam_theta_vec) * qSin(cam_pos_vec_[2]) * dV +
               1.0 * qSin(M_PI/2) * qSin(cam_phi_vec) * dH;
    double z = target_pos_[0] * qCos(target_pos_[1]) +
               1.0 * qCos(cam_theta_vec) * dV +
               1.0 * qCos(M_PI/2) * dH;

    target_pos_[0] = qSqrt(qPow(x, 2) + qPow(y, 2) + qPow(z, 2));
    target_pos_[1] = qAcos(z / target_pos_[0]);

    if(x == 0)
    {
        target_pos_[2] = 0;
    }
    else if(x < 0)
    {
        target_pos_[2] = qAtan(y / x) + M_PI;
    }
    else if(x > 0 && y < 0)
    {
        target_pos_[2] = 2 * M_PI + qAtan(y / x);
    }
    else
    {
        target_pos_[2] = qAtan(y / x);
    }
}

void SceneGL::initGlobalSceneList()
{
    global_scene_list_ = glGenLists(1);
    glNewList(global_scene_list_, GL_COMPILE);
        drawGround();
        loadTextures_logo();
        drawBarbedWire();
    glEndList();

}
/*
 * draw the ground with 160*100
 */
void SceneGL::drawGround()
{
    loadTextures_herbe();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        GLfloat mat_color_tex[] = {1.0 ,1.0 ,1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_tex);
            glBegin(GL_QUADS);
                glNormal3f(0,0,1);
                glTexCoord2f(0, 1);
                glVertex3f(-50, -10, 0);
                glTexCoord2f(1, 1);
                glVertex3f(50,-10, 0);
                glTexCoord2f(1, 0);
                glVertex3f(50,70, 0);
                glTexCoord2f(0, 0);
                glVertex3f(-50,70, 0);
            glEnd();
            glBegin(GL_QUADS);
                glNormal3f(0,0,1);
                glTexCoord2f(0, 0);
                glVertex3f(-50, 70, 0);
                glTexCoord2f(1, 0);
                glVertex3f(50,70, 0);
                glTexCoord2f(1, 1);
                glVertex3f(50,150, 0);
                glTexCoord2f(0, 1);
                glVertex3f(-50,150, 0);
            glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void SceneGL::drawBarbedWire()
{
    int x = -45, y = 5, flag_net = 0, flag_tex = 0;
    while(true)
    {
        flag_tex = 1 - flag_tex;
        drawStick(x, y);
        drawNet(x, y, flag_net);
        if(flag_tex)
            drawTexture_logo(x, y, flag_net);
        if(x == -45 && y < 145)
        {
            y += 5;
            flag_net = (y == 145) ? 1 : 0;
        }
        else if(y == 145 && x < 45)
        {
            x += 5;
            flag_net = (x == 45) ? 2 : 1;
        }
        else if(x == 45 && y > 5)
        {
            y -= 5;
            flag_net = (y == 5) ? 3 : 2;
        }
        else break;
    }

}

void SceneGL::drawStick(int x, int y)
{
    glPushMatrix();
        GLfloat mat_color_stick[] = {0.6 ,0.6 ,0.6, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_stick);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0, 1, 0);
            glVertex3f(x+0.1, y+0.1, 5);
            glVertex3f(x+0.1, y+0.1, 0);
            glVertex3f(x-0.1, y+0.1, 5);
            glVertex3f(x-0.1, y+0.1, 0);
            glNormal3f(-1, 0, 0);
            glVertex3f(x-0.1, y-0.1, 5);
            glVertex3f(x-0.1, y-0.1, 0);
            glNormal3f(0, -1, 0);
            glVertex3f(x+0.1, y-0.1, 5);
            glVertex3f(x+0.1, y-0.1, 0);
            glNormal3f(1, 0, 0);
            glVertex3f(x+0.1, y+0.1, 5);
            glVertex3f(x+0.1, y+0.1, 0);
        glEnd();
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glVertex3f(x+0.1, y+0.1, 5);
            glVertex3f(x-0.1, y+0.1, 5);
            glVertex3f(x-0.1, y-0.1, 5);
            glVertex3f(x+0.1, y-0.1, 5);
        glEnd();
    glPopMatrix();
}

void SceneGL::drawNet(float x, float y, int flag)
{
    if(flag == 3) return;
    glPushMatrix();
        GLfloat mat_color_net[] = {0.6 ,0.6 ,0.6, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_net);
        glBegin(GL_LINE_STRIP);
            for(int z = 0 ; z <= 5 ; z++)
            {
                glVertex3f(x, y, z);
                if(z % 2 == 0)
                    if(flag == 0)
                        y += 5;
                    else if(flag == 1)
                        x += 5;
                    else
                        y -= 5;
                else
                    if(flag == 0)
                        y -= 5;
                    else if(flag == 1)
                        x -= 5;
                    else
                        y += 5;
            }
            for(int z = 5 ; z >= 0 ; z--)
            {
                glVertex3f(x, y, z);
                if(z % 2 == 0)
                    if(flag == 0)
                        y -= 5;
                    else if(flag == 1)
                        x -= 5;
                    else
                        y += 5;
                else
                    if(flag == 0)
                        y += 5;
                    else if(flag == 1)
                        x += 5;
                    else
                        y -= 5;
            }
        glEnd();
        glBegin(GL_LINE_LOOP);
            float x_mid, y_mid;
            if(flag == 0)
            {
                x_mid = x;y_mid = y + 2.5;
                y += 5;
            }
            else if(flag == 1)
            {
                x_mid = x + 2.5;y_mid = y;
                x += 5;
            }
            else
            {
                x_mid = x;y_mid = y - 2.5;
                y -= 5;
            }
            glVertex3f(x_mid, y_mid, 0);
            for(float z = 0.5 ; z <= 4.5 ; z+=1)
            {
                glVertex3f(x, y, z);
                if((int)(z + 0.5) % 2 == 0)
                    if(flag == 0)
                        y += 5;
                    else if(flag == 1)
                        x += 5;
                    else
                        y -= 5;
                else
                    if(flag == 0)
                        y -= 5;
                    else if(flag == 1)
                        x -= 5;
                    else
                        y += 5;
            }
            glVertex3f(x_mid, y_mid, 5);
            for(float z = 4.5 ; z >= 0.5 ; z-=1)
            {
                glVertex3f(x, y, z);
                if((int)(z + 0.5) % 2 == 0)
                    if(flag == 0)
                        y -= 5;
                    else if(flag == 1)
                        x -= 5;
                    else
                        y += 5;
                else
                    if(flag == 0)
                        y += 5;
                    else if(flag == 1)
                        x += 5;
                    else
                        y -= 5;
            }
            glVertex3f(x_mid, y_mid, 0);
        glEnd();
    glPopMatrix();
}

void SceneGL::loadTextures_logo()
{
    QImage tex, buf;
    if(!buf.load(":/images/texture/Logo_TSE.png"))
    {
        qWarning("Cannot open the image!");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::white);
        buf = dummy;
    }
    tex = convertToGLFormat(buf);
    glGenTextures(1, &LOGO_texture_);
    glBindTexture(GL_TEXTURE_2D, LOGO_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void SceneGL::drawTexture_logo(float x, float y, int flag)
{
    if(flag == 3) return;
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        GLfloat mat_color_tex[] = {1.0 ,1.0 ,1.0, 0.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_tex);
        glBegin(GL_QUADS);
            if(flag == 0)
            {
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x, y + 0.5, 0.5);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x, y + 4.5, 0.5);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x, y + 4.5, 4.5);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x, y + 0.5, 4.5);
            }
            else if(flag == 1)
            {
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x + 0.5, y, 0.5);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x + 4.5, y, 0.5);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x + 4.5, y, 4.5);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x + 0.5, y, 4.5);
            }
            else
            {
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x, y - 0.5, 0.5);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x, y - 4.5, 0.5);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x, y - 4.5, 4.5);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x, y - 0.5, 4.5);
            }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void SceneGL::loadTextures_herbe(){
    QImage tex, buf;
    if(!buf.load(":/images/texture/herbe.jpg"))
    {
        qWarning("Cannot open the image!");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::white);
        buf = dummy;
    }
    tex = convertToGLFormat(buf);
    glGenTextures(1, &HERBE_texture_);
    glBindTexture(GL_TEXTURE_2D, HERBE_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void SceneGL::draw_circle(const GLfloat radius,const GLuint num_vertex)
{
    QImage tex, buf;
    if(!buf.load(":/images/texture/target.png"))
    {
        qWarning("Cannot open the image!");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::white);
        buf = dummy;
    }
    tex = convertToGLFormat(buf);
    glGenTextures(1, &TARGET_texture_);
    glBindTexture(GL_TEXTURE_2D, TARGET_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLfloat vertex[4];
    GLfloat texcoord[2];

    const GLfloat delta_angle = 2.0*M_PI/num_vertex;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,TARGET_texture_);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBegin(GL_TRIANGLE_FAN);

    //draw the vertex at the center of the circle
    texcoord[0] = 0.5;
    texcoord[1] = 0.5;
    glTexCoord2fv(texcoord);
    vertex[0] = vertex[1] = vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    //draw the vertex on the contour of the circle
    for(int i = 0; i < num_vertex ; i++)
    {
        texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
        texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
        glTexCoord2fv(texcoord);

        vertex[0] = std::cos(delta_angle*i) * radius;
        vertex[1] = std::sin(delta_angle*i) * radius;
        vertex[2] = 0.0;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }

    texcoord[0] = (1.0 + 1.0)*0.5;
    texcoord[1] = (0.0 + 1.0)*0.5;
    glTexCoord2fv(texcoord);

    vertex[0] = 1.0 * radius;
    vertex[1] = 0.0 * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void SceneGL::draw_target(){
    glPushMatrix();
        glTranslatef(round->getPositionX(),round->getPositionY(),0.1);
        switch (round->getLevel()) {
        case EASY:
            draw_circle(10,100);
            break;
        case NORMAL:
            draw_circle(7.5,100);
            break;
        case HARD:
            draw_circle(5,100);
            break;
        }
    glPopMatrix();
}

void SceneGL::initCatapult()
{
    catapult_status_ = new CatapulteStatus();
    catapult_base_ = glGenLists(1);
    glNewList(catapult_base_, GL_COMPILE);
        drawCatapultBase();
    glEndList();
    trebuchet_ = glGenLists(1);
    glNewList(trebuchet_, GL_COMPILE);
        drawTrebuchet();
    glEndList();
    trebuchet_load_ = glGenLists(1);
    glNewList(trebuchet_load_, GL_COMPILE);
        drawLoad();
    glEndList();
}

void SceneGL::drawCatapultBase()
{
    glPushMatrix();
        GLfloat mat_color_base[] = {0.7 ,0.4 ,0.0, 1.0};
//        GLfloat mat_color_base[] = {0.6 ,0.6 ,0.6, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_base);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0, 0, -1);
            glVertex3f(3, 6, 0);
            glVertex3f(-3, 6, 0);
            glVertex3f(3, -6, 0);
            glVertex3f(-3, -6, 0);
            glNormal3f(0, -1, 0);
            glVertex3f(3, -6, 0.2);
            glVertex3f(-3, -6, 0.2);
            glNormal3f(0, 0, 1);
            glVertex3f(3, 6, 0.2);
            glVertex3f(-3, 6, 0.2);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(-3, -6, 0);
            glVertex3f(-3, -6, 0.2);
            glVertex3f(-3, 6, 0);
            glVertex3f(-3, 6, 0.2);
            glNormal3f(0, 1, 0);
            glVertex3f(3, 6, 0);
            glVertex3f(3, 6, 0.2);
            glNormal3f(1, 0, 0);
            glVertex3f(3, -6, 0);
            glVertex3f(3, -6, 0.2);
        glEnd();
        drawHolder(-3);
        drawHolder(2.5);
        drawCylinder(0);
    glPopMatrix();
}

void SceneGL::drawHolder(float dx)
{
    glPushMatrix();
        glTranslatef(dx, 0 ,0);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(0, 6, 0.2);
            glVertex3f(0, 0.3, 4);
            glVertex3f(0, 5.4, 0.2);
            glVertex3f(0, 0.3, 3.6);
            glNormal3f(0, -6, -4);
            glVertex3f(0.5, 5.4, 0.2);
            glVertex3f(0.5, 0.3, 3.6);
            glNormal3f(1, 0, 0);
            glVertex3f(0.5, 6, 0.2);
            glVertex3f(0.5, 0.3, 4);
            glNormal3f(0, 6, 4);
            glVertex3f(0, 6, 0.2);
            glVertex3f(0, 0.3, 4);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(0, -6, 0.2);
            glVertex3f(0, -0.3, 4);
            glVertex3f(0, -5.4, 0.2);
            glVertex3f(0, -0.3, 3.6);
            glNormal3f(0, 6, -4);
            glVertex3f(0.5, -5.4, 0.2);
            glVertex3f(0.5, -0.3, 3.6);
            glNormal3f(1, 0, 0);
            glVertex3f(0.5, -6, 0.2);
            glVertex3f(0.5, -0.3, 4);
            glNormal3f(0, -6, 4);
            glVertex3f(0, -6, 0.2);
            glVertex3f(0, -0.3, 4);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0, -1, 0);
            glVertex3f(0, -0.3, 8.2);
            glVertex3f(0, -0.3, 0.2);
            glVertex3f(0.5, -0.3, 8.2);
            glVertex3f(0.5, -0.3, 0.2);
            glNormal3f(1, 0, 0);
            glVertex3f(0.5, 0.3, 8.2);
            glVertex3f(0.5, 0.3, 0.2);
            glNormal3f(0, 1, 0);
            glVertex3f(0, 0.3, 8.2);
            glVertex3f(0, 0.3, 0.2);
            glNormal3f(-1, 0, 0);
            glVertex3f(0, -0.3, 8.2);
            glVertex3f(0, -0.3, 0.2);
        glEnd();
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glVertex3f(0, 0.3, 8.2);
            glVertex3f(0, -0.3, 8.2);
            glVertex3f(0.5, -0.3, 8.2);
            glVertex3f(0.5, 0.3, 8.2);
        glEnd();
    glPopMatrix();
}

void SceneGL::drawCylinder(int flag)
{
    glPushMatrix();
        GLUquadric *cylinder = gluNewQuadric();
        if(flag == 0)
        {
            glTranslatef(-2.5, 0, 8);
            glRotatef(90, 0, 1, 0);
            gluCylinder(cylinder,0.2,0.2,5,32,32);
        }
        else
        {
            glTranslatef(-0.4, 0, 0);
            glRotatef(90, 0, 1, 0);
            gluCylinder(cylinder,0.1,0.1,0.8,32,32);
        }
        gluDeleteQuadric(cylinder);
    glPopMatrix();
}

void SceneGL::drawSphere()
{
    glPushMatrix();
        GLfloat mat_color_sphere[] = {0.4 ,0.4 ,0.4, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_sphere);
        GLUquadric *sphere = gluNewQuadric();
        gluSphere(sphere, 0.5, 32, 32);
        gluDeleteQuadric(sphere);
    glPopMatrix();
}

void SceneGL::drawConnecter()
{
    glPushMatrix();
        GLfloat mat_color_connecter[] = {0.4 ,0.4 ,0.4, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_connecter);
        glBegin(GL_LINE_STRIP);
            if(!catapult_status_->AngleTrebuchetReady())
            {
                if(catapult_status_->getAngleTrebuchet() >= 0)
                {
                    glVertex3f(0, catapult_status_->getSphereYPos(), -7.5);
                }
                else
                {
                    glVertex3f(0, catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI), catapult_status_->getTrebuchetBottomZPos() - qCos(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5);
                }
            }
            else
            {
                glVertex3f(0, catapult_status_->getTrebuchetBottomYPos() + 7.5 * qSin(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI), catapult_status_->getTrebuchetBottomZPos() - qCos(catapult_status_->getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5);
            }
            glVertex3f(0, catapult_status_->getTrebuchetBottomYPos(), catapult_status_->getTrebuchetBottomZPos());
        glEnd();
    glPopMatrix();
}

void SceneGL::drawTrebuchet()
{
    glPushMatrix();
        GLfloat mat_color_trebuchet[] = {0.7 ,0.4 ,0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_trebuchet);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0, 0, 1);
            glVertex3f(0.3, 5, 0.3);
            glVertex3f(-0.3, 5, 0.3);
            glVertex3f(0.3, -17, 0.3);
            glVertex3f(-0.3, -17, 0.3);
            glNormal3f(0, -1, 0);
            glVertex3f(0.3, -17, -0.3);
            glVertex3f(-0.3, -17, -0.3);
            glNormal3f(0, 0, -1);
            glVertex3f(0.3, 5, -0.3);
            glVertex3f(-0.3, 5, -0.3);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(-0.3, -17, 0.3);
            glVertex3f(-0.3, -17, -0.3);
            glVertex3f(-0.3, 5, 0.3);
            glVertex3f(-0.3, 5, -0.3);
            glNormal3f(0, 1, 0);
            glVertex3f(0.3, 5, 0.3);
            glVertex3f(0.3, 5, -0.3);
            glNormal3f(1, 0, 0);
            glVertex3f(0.3, -17, 0.3);
            glVertex3f(0.3, -17, -0.3);
        glEnd();
    glPopMatrix();
}

void SceneGL::drawLoad()
{
    GLfloat mat_color_load[] = {0.7 ,0.4 ,0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_load);
    glPushMatrix();
        drawCylinder(1);
        drawholder(-0.5);
        drawholder(0.4);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0, 0, 1);
            glVertex3f(0.5, 1, -1);
            glVertex3f(-0.5, 1, -1);
            glVertex3f(0.5, -1, -1);
            glVertex3f(-0.5, -1, -1);
            glNormal3f(0, -1, 0);
            glVertex3f(0.5, -1, -2);
            glVertex3f(-0.5, -1, -2);
            glNormal3f(0, 0, -1);
            glVertex3f(0.5, 1, -2);
            glVertex3f(-0.5, 1, -2);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(-0.5, -1, -1);
            glVertex3f(-0.5, -1, -2);
            glVertex3f(-0.5, 1, -1);
            glVertex3f(-0.5, 1, -2);
            glNormal3f(0, 1, 0);
            glVertex3f(0.5, 1, -1);
            glVertex3f(0.5, 1, -2);
            glNormal3f(1, 0, 0);
            glVertex3f(0.5, -1, -1);
            glVertex3f(0.5, -1, -2);
        glEnd();
    glPopMatrix();
}

void SceneGL::drawholder(float dx)
{
    glPushMatrix();
        glTranslatef(dx, 0 ,0);
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(0, 0, qSqrt(2) * 0.1);
            glVertex3f(0, 1, -1);
            glVertex3f(0, 0, -qSqrt(2) * 0.1);
            glVertex3f(0, 1 - qSqrt(2) * 0.2, -1);
            glNormal3f(0, -1, -1);
            glVertex3f(0.1, 0, -qSqrt(2) * 0.1);
            glVertex3f(0.1, 1 - qSqrt(2) * 0.2, -1);
            glNormal3f(1, 0, 0);
            glVertex3f(0.1, 0, qSqrt(2) * 0.1);
            glVertex3f(0.1, 1, -1);
            glNormal3f(0, 1, 1);
            glVertex3f(0, 0, qSqrt(2) * 0.1);
            glVertex3f(0, 1, -1);
        glEnd();
        glBegin(GL_QUAD_STRIP);
            glNormal3f(-1, 0, 0);
            glVertex3f(0, 0, qSqrt(2) * 0.1);
            glVertex3f(0, -1, -1);
            glVertex3f(0, 0, -qSqrt(2) * 0.1);
            glVertex3f(0, qSqrt(2) * 0.2 - 1, -1);
            glNormal3f(0, 1, -1);
            glVertex3f(0.1, 0, -qSqrt(2) * 0.1);
            glVertex3f(0.1, qSqrt(2) * 0.2 - 1, -1);
            glNormal3f(1, 0, 0);
            glVertex3f(0.1, 0, qSqrt(2) * 0.1);
            glVertex3f(0.1, -1, -1);
            glNormal3f(0, -1, 1);
            glVertex3f(0, 0, qSqrt(2) * 0.1);
            glVertex3f(0, -1, -1);
        glEnd();
    glPopMatrix();
}
