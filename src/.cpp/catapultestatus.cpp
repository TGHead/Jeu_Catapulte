#include "catapultestatus.h"

CatapulteStatus::CatapulteStatus()
{
    angle_H_ = 0;
    angle_trebuchet_ = qAsin(8.0 / 17.0) / M_PI * 180.0;
}

void CatapulteStatus::setAngleH(float h)
{
    angle_H_ = 90 * (0.5 - h);
}

void CatapulteStatus::setAngleTrebuchet(float v)
{
    angle_trebuchet_ = qAsin(8.0 / 17.0) / M_PI * 180.0 * v;
}
