#ifndef CATAPULTESTATUS_H
#define CATAPULTESTATUS_H

#include <QtMath>

class CatapulteStatus
{

private:
    double angle_H_;
    double angle_trebuchet_;

public:
    CatapulteStatus();
    double getAngleH() { return angle_H_; }
    double getAngleTrebuchet() { return angle_trebuchet_; }

};

#endif // CATAPULTESTATUS_H
