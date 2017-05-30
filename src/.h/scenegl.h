#ifndef SCENEGL_H
#define SCENEGL_H

#include <QGLWidget>
#include <openGL/glu.h>
#include <QtOpenGL>
#include "catapultestatus.h"
#include "gameround.h"

class SceneGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit SceneGL(QWidget *parent = 0);
    ~SceneGL();
    CatapulteStatus* getCatapulteStatus() {return catapult_status_;}
    void setCatapultAngle(float h, float v);
    void setRound(GameRound *R){round=R;}
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
    void loadTextures_logo();
    void drawTexture_logo(float x, float y, int flag);
    void loadTextures_herbe();
    void draw_circle(const GLfloat radius,const GLuint num_vertex);
    void draw_target();

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
    GLuint TARGET_texture_;
    GLuint HERBE_texture_;

    GLuint global_scene_list_;
    GLuint catapult_base_;
    GLuint trebuchet_load_;
    GLuint trebuchet_;
    GameRound *round;
};

#endif // SCENEGL_H
