#include "src/.h/catapultestatus.h"
#include <QDebug>

CatapulteStatus::CatapulteStatus()
{
    float delta = 0.58734469222569181677479231654031;
    angle_H_ = 0;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0;//delta = 0.58734469222569181677479231654031
    calAngleSphere();
    H_speed_ = 0;
}

void CatapulteStatus::setAngleH(float h)
{
    angle_H_ = -65.48 * (0.5 - h);
}

void CatapulteStatus::setAngleTrebuchet(float v)
{
    float delta = 0.58734469222569181677479231654031;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0 * v;
    calAngleSphere();
}

void CatapulteStatus::setAngleTrebuchet(double angle)
{
    angle_trebuchet_ = angle;
    calAngleSphere();
}

void CatapulteStatus::calAngleSphere()
{
    if(angle_trebuchet_ >= 0)
        angle_sphere_ = qAcos(1.0 - 17.0 * qSin(angle_trebuchet_ / 180.0 * M_PI) / 7.5) / M_PI * 180.0 - angle_trebuchet_ + 90.0;
    else
        angle_sphere_ = 90.0 + angle_trebuchet_;
}

void CatapulteStatus::setHSpeed()
{
    float delta = 0.58734469222569181677479231654031;
    H_speed_ = 27.2 + (angle_backup[1]/(qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0)) * 31.2;
    Sphere_Z_ = 7.5 + getTrebuchetBottomZPos() - qCos(getAngleTrebuchet() * 2 / 180.0 * M_PI) * 7.5;
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

bool CatapulteStatus::AngleTrebuchetReady()
{
    if(angle_trebuchet_ <= -90.0)
    {
        return true;
    }
}

bool CatapulteStatus::SphereOutofBounds()
{
    if(Sphere_Z_ <= 0.5)
    {
        return true;
    }
    else if(H_speed_ * qSqrt( 2 * (32.5 - Sphere_Z_) / 9.8) * qSin(angle_H_ / 180.0 * M_PI) >= 45)
    {
        return true;
    }
    else
    {
        return false;
    }
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
