//
// Created by sw on 24-3-27.
//
#pragma once
#include "armquadrover_msgs/msg/ddsm115_servo.h"
#include "std_msgs/msg/detail/int16_multi_array__struct.h"

#ifdef __cplusplus
extern "C" {
#endif
void Arm1_setTargetAngle(size_t size,double *data);
std_msgs__msg__Int16MultiArray Arm1_getCurrentAngle(void);

#ifdef __cplusplus
}
#endif
