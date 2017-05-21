#include "catapultestatus.h"

CatapulteStatus::CatapulteStatus()
{
    angle_H_ = 0;
    angle_trebuchet_ = qAsin(8.0 / 17.0) / M_PI * 180.0;
}
