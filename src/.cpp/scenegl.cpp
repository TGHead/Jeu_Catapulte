#include "scenegl.h"

SceneGL::SceneGL(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    /* [0]:modulo( r ),
     * [1]:angle( theta ),
     * [2]:angle( phi )
        unit of angle : rad*/
    pos_cam_[0] = 10.0;pos_cam_[1] = M_PI / 2;pos_cam_[2] = 0.0;
    vec_Head_[0] = 1.0;vec_Head_[1] = 0.0;vec_Head_[2] = 0.0;
}

SceneGL::~SceneGL()
{

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

    rotate_camera();
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
    double dphi = 0 - ((double)(event->x() - lastPos_.x()) / (double) width()) * M_PI;
    double dtheta = 0 - ((double)(event->y() - lastPos_.y()) / (double) width()) * M_PI;

    calcul_cam_pos(dtheta, dphi);

    if (event->buttons() & Qt::LeftButton)
    {
        updateGL();
    }

    lastPos_ = event->pos();
}

void SceneGL::wheelEvent(QWheelEvent *event)
{
    pos_cam_[0] -= event->delta()/120;
    if(pos_cam_[0] < 0.1)
    {
        pos_cam_[0] = 0.1;
    }
    else if(pos_cam_[0] > 200)
    {
        pos_cam_[0] = 200;
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

void SceneGL::rotate_camera()
{
    gluLookAt(pos_cam_[0] * qSin(pos_cam_[1]) * qCos(pos_cam_[2]),
              pos_cam_[0] * qSin(pos_cam_[1]) * qSin(pos_cam_[2]),
              pos_cam_[0] * qCos(pos_cam_[1]),
            0.0,0.0,0.0,
            vec_Head_[0] * qSin(vec_Head_[1]) * qCos(vec_Head_[2]),
            vec_Head_[0] * qSin(vec_Head_[1]) * qSin(vec_Head_[2]),
            vec_Head_[0] * qCos(vec_Head_[1]));
}

void SceneGL::calcul_cam_pos(double dtheta, double dphi)
{
    pos_cam_[1] += dtheta;
    pos_cam_[2] += dphi;
    for(int i = 1; i < 3; i++)
    {
        if(pos_cam_[i] < 0)
        {
            pos_cam_[i] += 2 * M_PI;
        }
        else if(pos_cam_[i] > 2 * M_PI)
        {
            pos_cam_[i] -= 2 * M_PI;
        }
    }

    vec_Head_[1] += dtheta;
    vec_Head_[2] += dphi;
    for(int i = 1; i < 3; i++)
    {
        if(vec_Head_[i] < 0)
        {
            vec_Head_[i] += 2 * M_PI;
        }
        else if(vec_Head_[i] > 2 * M_PI)
        {
            vec_Head_[i] -= 2 * M_PI;
        }
    }
}
