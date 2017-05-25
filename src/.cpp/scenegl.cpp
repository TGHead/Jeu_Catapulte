#include "src/.h/scenegl.h"

SceneGL::SceneGL(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    /* [0]:modulo( r ),
     * [1]:angle( theta ),
     * [2]:angle( phi )
        unit of angle : rad*/
    target_pos_[0] = 0.0; target_pos_[1] = 0.0; target_pos_[2] = 0.0;
    cam_pos_vec_[0] = 100.0;cam_pos_vec_[1] = M_PI / 3;cam_pos_vec_[2] = M_PI * 3 / 2;

    catapult_status_ = NULL;
}

SceneGL::~SceneGL()
{
    if(catapult_status_ != NULL)
        delete catapult_status_;

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

}

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

    loadTextures();
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
                glRotatef(catapult_status_->getAngleTrebuchet(), 1, 0, 0);
                glCallList(trebuchet_);
            glPopMatrix();
            glTranslatef(0, 5 * qCos(catapult_status_->getAngleTrebuchet() / 180.0 * M_PI), 5 * qSin(catapult_status_->getAngleTrebuchet() / 180.0 * M_PI));
            glCallList(trebuchet_load_);
        glPopMatrix();
    glPopMatrix();
}

void SceneGL::move_camera()
{
    double vec_up_theta = cam_pos_vec_[1] - M_PI / 2;
    if(vec_up_theta < 0)
    {
        vec_up_theta += 2 * M_PI;
    }
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
//    qDebug()<<cam_pos_vec_[1]<<" "<<cam_pos_vec_[2];
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
        drawBarbedWire();
    glEndList();
}

void SceneGL::drawGround()
{
    glPushMatrix();
//        glEnable(GL_COLOR_MATERIAL);
//        glColorMaterial(GL_FRONT,GL_AMBIENT);
        GLfloat mat_color_test[] = {0.0 ,1.0 ,0.0, 1.0};
//        GLfloat mat_shininess[] = {64.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_test);
//        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//        qglColor(Qt::red);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glVertex3f(-50,-10,0);
            glVertex3f(50,-10,0);
            glVertex3f(50,100,0);
            glVertex3f(-50,100,0);
        glEnd();
//            glDisable(GL_COLOR_MATERIAL);
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
            drawTexture(x, y, flag_net);
        if(x == -45 && y < 95)
        {
            y += 5;
            flag_net = (y == 95) ? 1 : 0;
        }
        else if(y == 95 && x < 45)
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

void SceneGL::loadTextures()
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


void SceneGL::drawTexture(float x, float y, int flag)
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
//        delete cylinder;
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
