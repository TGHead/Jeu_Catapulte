#ifndef CATAPULTESTATUS_H
#define CATAPULTESTATUS_H

#include <QtMath>

/*
 * This class is used to record the status parameters of the catapult like horizontal angle and vertical angle etc...
 * Created an instance while initializing OpenGL Scene in the object SceneGL_
 */

class CatapulteStatus
{

private:
    double angle_H_;// catapult horizontal angle, available value: [-32.74, 32.74] unit:DEG
    double angle_trebuchet_;// catapult vertical angle, available value: [-90, 27.06] unit:DEG
    double angle_sphere_;// a relative angle between the projectile and the arm of the catapult, available value: [0, 153) unit:DEG

    double angle_backup[3];// an double to store 3 angles above when catapult receives the "Fire" signal and begin to draw animation. 3 angles will be recovered while clicking replay button or next target button.

    double H_speed_;// storing the projectile horizontal velocity
    double Sphere_Z_;// storing the height of projectile used in the 3rd launch phase of launching process

private:
    void calAngleSphere();// a private function to calculate angle_sphere_ according to the angle_trebuchet_

public:
    CatapulteStatus();

    /*
     * Getters
     */

    double getAngleH() { return angle_H_; }
    double getAngleTrebuchet() { return angle_trebuchet_; }
    double getAngleSphere() { return angle_sphere_; }

    /*
     * Setters
     */

    void setAngleH(float h);// used to set the angle_H_ into [-32.74, 32.74] covering the target area. param h is a float variable between 0 and 1, which store the relative horizontal position of template image in the whole camera area, this variable is captured in function afficherImage() of Catapulte class.
    void setAngleTrebuchet(float v);// used to set the angle_trebuchet_ into [0, 27.06] during the aimming stage. param v is similar with param h above.
    void setAngleTrebuchet(double angle);// Override function of the function above, used in drawAnime() function of the SceneGL class while the 1st and 2nd launch phase, param angle is an angle of unit DEG
    void setHSpeed();// this function is used to set horizontal velocity of projectile in drawAnime() function of the SceneGL class while passing the 2nd launch phase to the 3rd launch phase

    double getSphereYPos();// used to get the relative Y position of the projectile in draw and move_camera function of SceneGL class except during the 2nd and 3rd launch phase
    double getTrebuchetBottomYPos();// used to get the relative Y position of the long part of trebuchet arm during the 2nd launch phase
    double getTrebuchetBottomZPos();// used to get the relative Z position of the long part of trebuchet arm during the 2nd launch phase

    /*
     * Getters
     */

    double getHSpeed(){ return H_speed_; }
    double getSphereZ() {return Sphere_Z_;}
    void SphereZDecrement(double delta){ Sphere_Z_ -= delta; }

    bool AngleTrebuchetReady();// used to check if the trebuchet arm is perpendicular to the ground which means it returns true while passing to the 3rd launch phase.
    bool SphereOutofBounds();//  used to check if the projectile falls on the ground or runs out of bounds

    void AngleBackUp();// backup 3 angles to the array angle_backup[3]
    void AngleRecover();// recover 3 angles from array angle_backup[3]
};

#endif // CATAPULTESTATUS_H
