#include "catapultestatus.h"

CatapulteStatus::CatapulteStatus()
{
    const float delta = 0.58734469222569181677479231654031;
    angle_H_ = 0;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0;//delta = 0.58734469222569181677479231654031
}

void CatapulteStatus::setAngleH(float h)
{
    angle_H_ = -90 * (0.5 - h);
}

void CatapulteStatus::setAngleTrebuchet(float v)
{
    const float delta = 0.58734469222569181677479231654031;
    angle_trebuchet_ = qAsin(8.0 / (17.0 + delta)) / M_PI * 180.0 * v;
}
