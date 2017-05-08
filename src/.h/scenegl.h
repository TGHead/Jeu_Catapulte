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
    void move_camera();
    void rotate_camera(double X, double Y);
    void translate_camera(double X, double Y);

    double target_pos_[3];// record the center of the viewport which the camera point to.
    double cam_pos_vec_[3]; //record a vector which points to the camera's position from the target center.
    QPoint lastPos_;
};

#endif // SCENEGL_H
