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

    void initGlobalSceneList();
    void drawGround();
    void drawBarbedWire();
    void drawStick(int x, int y);
    void drawNet(float x, float y, int flag);

    double target_pos_[3];// record the center of the viewport which the camera point to.
    double cam_pos_vec_[3]; //record a vector which points to the camera's position from the target center.
    QPoint lastPos_;

    GLuint global_scene_list_;
};

#endif // SCENEGL_H
