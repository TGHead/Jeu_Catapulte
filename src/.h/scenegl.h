#ifndef SCENEGL_H
#define SCENEGL_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QtOpenGL>

class SceneGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit SceneGL(QWidget *parent = 0);
    ~SceneGL();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void draw();
    void rotate_camera();
    void calcul_cam_pos(double dtheta, double dphi);

    double vec_Head_[3];
    double pos_cam_[3];
    QPoint lastPos_;
};

#endif // SCENEGL_H
