#pragma once
#include "armquadrover_msgs//msg/ddsm115.h"
#ifdef __cplusplus
extern "C" {
#endif
void setTargetSpeed(int16_t speed1,int16_t speed2,int16_t speed3,int16_t speed4,float angle1,float angle2,float angle3,float angle4);
armquadrover_msgs__msg__DDSM115 getCurrentSpeed(void);

#ifdef __cplusplus
}
#endif