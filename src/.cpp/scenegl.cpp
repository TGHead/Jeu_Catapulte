#include "src/.h/scenegl.h"

SceneGL::SceneGL(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    /* [0]:modulo( r ),
     * [1]:angle( theta ),
     * [2]:angle( phi )
        unit of angle : rad*/
    target_pos_[0] = 0.0; target_pos_[1] = 0.0; target_pos_[2] = 0.0;
    cam_pos_vec_[0] = 10.0;cam_pos_vec_[1] = M_PI / 2;cam_pos_vec_[2] = 0.0;
}

SceneGL::~SceneGL()
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

//    GLfloat global_light_position[] = { 0.0f ,0.0f ,1.0f ,0.0f };
    GLfloat global_light_ambient[] = { 1.0f ,1.0f ,1.0f ,1.0f };
//    GLfloat global_light_diffuse[] = { 1.0f ,1.0f ,1.0f ,1.0f };

//    glLightfv(GL_LIGHT0, GL_POSITION, global_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, global_light_ambient);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
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
    gluPerspective(30, (double)width/height, .1, 200);

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
//    qglColor(Qt::red);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
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
