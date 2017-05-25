#ifndef CATAPULTESTATUS_H
#define CATAPULTESTATUS_H

#include <QtMath>

class CatapulteStatus
{

private:
    double angle_H_;
    double angle_trebuchet_;
    double angle_sphere_;

private:
    void calAngleSphere();

public:
    CatapulteStatus();
    double getAngleH() { return angle_H_; }
    double getAngleTrebuchet() { return angle_trebuchet_; }
    void setAngleH(float h);
    void setAngleTrebuchet(float v);

    double getSphereYPos();
    double getTrebuchetBottomYPos();
    double getTrebuchetBottomZPos();
};

#endif // CATAPULTESTATUS_H
