#include "src/.h/catapultestatus.h"

CatapulteStatus::CatapulteStatus()
{
    const float delta = 0.58734469222569181677479231654031;
    angle_H_ = 0;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0;//delta = 0.58734469222569181677479231654031
    calAngleSphere();
}

void CatapulteStatus::setAngleH(float h)
{
    angle_H_ = -90 * (0.5 - h);
}

void CatapulteStatus::setAngleTrebuchet(float v)
{
    const float delta = 0.58734469222569181677479231654031;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0 * v;
    calAngleSphere();
}

void CatapulteStatus::calAngleSphere()
{
    if(angle_trebuchet_ >= 0)
        angle_sphere_ = qAcos(1.0 - 17.0 * qSin(angle_trebuchet_ / 180.0 * M_PI) / 7.5) / M_PI * 180.0 - angle_trebuchet_ + 90.0;
    else
        angle_sphere_ = 90.0 + angle_trebuchet_;
}

double CatapulteStatus::getSphereYPos()
{
    double a = 17.0 * qCos(angle_trebuchet_ / 180.0 * M_PI);
    double b = 7.5 * qSin((angle_sphere_ - 90.0 + angle_trebuchet_) / 180.0 * M_PI);
    return b - a;
}

double CatapulteStatus::getTrebuchetBottomYPos()
{
    return -17.0 * qCos(angle_trebuchet_ / 180.0 * M_PI);
}

double CatapulteStatus::getTrebuchetBottomZPos()
{
    return -17.0 * qSin(angle_trebuchet_ / 180.0 * M_PI);
}

void CatapulteStatus::AngleBackUp()
{
    angle_backup[0] = angle_H_;
    angle_backup[1] = angle_trebuchet_;
    angle_backup[2] = angle_sphere_;
}

void CatapulteStatus::AngleRecover()
{
    angle_H_ = angle_backup[0];
    angle_trebuchet_ = angle_backup[1];
    angle_sphere_ = angle_backup[2];
}
