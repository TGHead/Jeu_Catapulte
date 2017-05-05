#include "scenegl.h"

SceneGL::SceneGL(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

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

//    glPushMatrix ();
//        glDisable (GL_LIGHTING);
//        glTranslated(0.0f, 0.0f, 0.0f);
//        glColor3f (0.0f, 1.0f, 1.0f);
//        testdraw();
//        glEnable (GL_LIGHTING);
//    glPopMatrix ();

//    glPushMatrix ();
//        glRotated (0.0f, 1.0f, 0.0f, 0.0f);
        testdraw();
//        glFlush();
//    glPopMatrix ();
}

void SceneGL::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-2, +2, -2, +2, 0.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt (0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f);
    glLoadIdentity();
}

void SceneGL::mousePressEvent(QMouseEvent *event)
{

}

void SceneGL::mouseMoveEvent(QMouseEvent *event)
{

}

void SceneGL::testdraw()
{
//    qglColor(Qt::red);
//    glBegin(GL_QUAD_STRIP);
//        glNormal3f(0,0,-1);
//        glColor3f(1.0f,1.0f,1.0f);
//        glVertex3f(.5, -.5, .5);
//        glVertex3f(.5, .5, .5);
//        glVertex3f(.5 ,-.5 ,0);
//        glVertex3f(.5 ,.5 ,0);
//        glVertex3f(-.5 ,-.5 ,0);
//        glVertex3f(.5 ,-.5 ,0);
//        glVertex3f(-.5 ,-.5 ,.5);
//        glVertex3f(.5 ,-.5 ,.5);
//    glEnd();
    qglColor(Qt::red);
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
