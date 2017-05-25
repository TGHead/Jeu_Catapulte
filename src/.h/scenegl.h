#ifndef SCENEGL_H
#define SCENEGL_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QtOpenGL>
#include "catapultestatus.h"

class SceneGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit SceneGL(QWidget *parent = 0);
    ~SceneGL();
    void setCatapultAngle(float h, float v);
    void drawAnime();

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
    void loadTextures();
    void drawTexture(float x, float y, int flag);

    void initCatapult();
    void drawCatapultBase();
    void drawHolder(float dx);
    void drawCylinder(int flag);
    void drawSphere();
    void drawConnecter();

    void drawTrebuchet();
    void drawLoad();
    void drawholder(float dx);

    CatapulteStatus *catapult_status_;

    double target_pos_[3];// record the center of the viewport which the camera point to.
    double cam_pos_vec_[3]; //record a vector which points to the camera's position from the target center.
    QPoint lastPos_;

    GLuint LOGO_texture_;

    GLuint global_scene_list_;
    GLuint catapult_base_;
    GLuint trebuchet_load_;
    GLuint trebuchet_;
};

#endif // SCENEGL_H
