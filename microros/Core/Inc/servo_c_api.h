#pragma once
#include "armquadrover_msgs//msg/ddsm115_servo.h"
#ifdef __cplusplus
extern "C" {
#endif
void setTargetAngle(float angle1,float angle2,float angle3,float angle4);
armquadrover_msgs__msg__DDSM115Servo getCurrentAngle(void);

#ifdef __cplusplus
}
#endif