#ifndef CATAPULTESTATUS_H
#define CATAPULTESTATUS_H

#include <QtMath>

class CatapulteStatus
{

private:
    double angle_H_;
    double angle_trebuchet_;
    double angle_sphere_;

    double angle_backup[3];

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

    double getSphereYPos();
    double getTrebuchetBottomYPos();
    double getTrebuchetBottomZPos();

    bool AngleTrebuchetReady();

    void AngleBackUp();
    void AngleRecover();
};

#endif // CATAPULTESTATUS_H
