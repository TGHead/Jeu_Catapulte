#ifndef SCENEGL_H
#define SCENEGL_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QtOpenGL>
#include "catapultestatus.h"
#include "gameround.h"

/*
 * This class is designed to control and display the whole game area which is based on a QGLWidget, using functions of OpenGL
 *
 */

class SceneGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit SceneGL(QWidget *parent = 0);
    ~SceneGL();
    CatapulteStatus* getCatapulteStatus() {return catapult_status_;}// return the pointer of object catapult_status_
    void setCatapultAngle(float h, float v);// set catapult angle for display by calling functions setAngleH and setAngleTrebuchet of CatapulteStatus class
    void setRound(GameRound *R){round=R;}// associate GameRound pointer in the Catapulte class

    /*
     * 3 launch phase:
     *
     * 1: angle_trebuchet > 0, during this phase, the trebuchet arm is beginning to rotate but the projectile stays always on the ground
     * 2: angle_trebuchet > -90, during this phase, the trebuchet arm begins to have the projectile rotate with itself together, the projectile leaves the ground and begins to rotate around the end of trebuchet arm
     * 3: angle_trebuchet <= -90, during this phase, the trebuchet arm stops to rotate, and throws out the projectile, the projectile begins to a free fall drop with a horizontal velocity until it falls to the ground
     *
     */

    void drawAnime();//the principal function to display animation when "Fire" signal captured
    void setFiringFlag(bool flag) { firing_ = flag; }// set firing_ variable
//    void cameraBackup();
//    void cameraRecover();

protected:
    void initializeGL();// virtual function defined in QGLWidget, called once before the first call to paintGL() or resizeGL()
    void paintGL();// virtual function is called whenever the widget needs to be painted, override to add camera control and display function
    void resizeGL(int width, int height);// virtual function is called whenever the widget has been resized, override to set the perspective projection

    void mousePressEvent(QMouseEvent *event);// virtual function, to record the mouse position
    void mouseMoveEvent(QMouseEvent *event);// virtual function, to control camera movement, leftbutton to rotate, rightbutton to translate
    void wheelEvent(QWheelEvent *event);// virtual function, to control the distance between camera and camera center

private:
    void draw();// the principal function to display the whole scene, called by paintGL()
    void move_camera();// the principal function to control camera, called by paintGL()
    void rotate_camera(double X, double Y);// this function is to rotate camera while pressing and moving the leftbutton on the scene, param X, Y are coordinates of mouse on the widget, called by mouseMoveEvent function
    void translate_camera(double X, double Y);// this function is to translate camera while pressing and moving the rightbutton on the scene, param X, Y are coordinates of mouse on the widget, called by mouseMoveEvent function

    void initGlobalSceneList();// this function is to initialize the global scene list which include grass ground, barbed wire and the texture of logo TSE, called by initializeGL()
    void drawGround();// this function is to draw grass ground which uses 2 textures of grass image, called by initGlobalSceneList()
    void drawBarbedWire();// this function is to draw barbed wire with the logo TSE on it, called by initGlobalSceneList()
    void drawStick(int x, int y);// this function is to draw a single stick of barbed wire param x, y are coordinates of the scene, called by drawBarbedWire()
    void drawNet(float x, float y, int flag);// this function is to draw the barbed wire between 2 sticks, params x, y are coordinates of the scene, flag is used to control the 2 coordinates while painting, called by drawBarbedWire()
    void loadTextures_logo();// this funciton is to load the logo texture from an image, called by drawBarbedWire()
    void drawTexture_logo(float x, float y, int flag);// this function is to draw logo texture on barbed wire, param x,y,flag are same with drawNet function, called by drawNet(float x, float y, int flag)
    void loadTextures_herbe();// this funciton is to load the grass texture from an image, called by drawGround()
    void draw_circle(const GLfloat radius,const GLuint num_vertex);// this function is to draw target, with load texture process in it, param radius means the radius of target on the scene, num_vertex means the number of a circle divided, called by draw_target() function
    void draw_target();// this function is to display target on the scene according to the position recorded in the "round" object

    void initCatapult();// this function is to initialize 3 diaplay list of catapult include catapult base, trebuchet arm and its counterweight， called by initializeGL()
    void drawCatapultBase();// this function is to draw the base of the catapult, called by initCatapult()
    void drawHolder(float dx);// this function is to draw left and right holder of catapult, param dx is used to control the translation distance of holder, called by initCatapult()
    void drawCylinder(int flag);// this function is to draw the axle of trebuchet arm or the axle of counterweight, param flag==0 is the axle of trebuchet, flag == 1 is the axle of counterweight, called by drawCatapultBase() and drawLoad()
    void drawSphere();// this function is to draw projectile sphere, called by draw()
    void drawConnecter();// this function is to draw the wire between projectile and trebuchet arm, called by draw()

    void drawTrebuchet();// this function is to draw trebuchet arm, called by initCatapult()
    void drawLoad();// this function is to draw counterweight, called by initCatapult()
    void drawholder(float dx);// this function is to draw holder of counterweight, param dx is same with function drawHolder(float dx), called by drawLoad()

    CatapulteStatus *catapult_status_;// CatapulteStatus object pointer, making instance in initCatapult() function

    /* [0]:modulo( r ),
     * [1]:angle( theta ),
     * [2]:angle( phi )
        unit of angle : rad
     */

    double target_pos_[3];// record the center of the viewport which the camera point to.
    double cam_pos_vec_[3]; //record a vector which points to the camera's position from the target center.

    /*
     * Backup 2 array above when displaying animation
     */

    double target_pos_backup_[3];
    double cam_pos_vec_backup_[3];

    QPoint lastPos_;// record the last coordinates of the mouse on the widget

    /*
     * GLuint number to indicate textures
     */

    GLuint LOGO_texture_;
    GLuint TARGET_texture_;
    GLuint HERBE_texture_;

    /*
     * GLuint number to indicate display list
     */

    GLuint global_scene_list_;
    GLuint catapult_base_;
    GLuint trebuchet_load_;
    GLuint trebuchet_;

    GameRound *round;// point to the GameRound object in Catapulte class
    bool firing_;// boolean variable to indicate if it's displaying the animation
};

#endif // SCENEGL_H
