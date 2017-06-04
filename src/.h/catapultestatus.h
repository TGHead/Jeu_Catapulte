#ifndef CATAPULTESTATUS_H
#define CATAPULTESTATUS_H

#include <QtMath>

/*
 * This class is used to record the status parameters of the catapult like horizontal angle and vertical angle etc...
 */

class CatapulteStatus
{

private:
    double angle_H_;
    double angle_trebuchet_;
    double angle_sphere_;

    double angle_backup[3];

    double H_speed_;
    double Sphere_Z_;

private:
    void calAngleSphere();

public:
    CatapulteStatus();
    double getAngleH() { return angle_H_; }
    double getAngleTrebuchet() { return angle_trebuchet_; }
    double getAngleSphere() { return angle_sphere_; }
    void setAngleH(float h);
    void setAngleTrebuchet(float v);
    void setAngleTrebuchet(double angle);
    void setHSpeed();

    double getSphereYPos();
    double getTrebuchetBottomYPos();
    double getTrebuchetBottomZPos();

    double getHSpeed(){ return H_speed_; }
    double getSphereZ() {return Sphere_Z_;}
    void SphereZDecrement(double delta){ Sphere_Z_ -= delta; }

    bool AngleTrebuchetReady();
    bool SphereOutofBounds();

    void AngleBackUp();
    void AngleRecover();
};

#endif // CATAPULTESTATUS_H
