//
// Created by sw on 24-3-27.
//
#include "arm_servo_c_api.h"
#include "arm_servo.h"
#include "std_msgs/msg/detail/int16_multi_array__struct.h"

#ifdef __cplusplus
extern "C" {
#endif
            arm_servo arm1_servo(0x01,&huart4);

void Arm1_setTargetAngle(size_t size,double *data)
{
    for (int i = 0; i < size; i++) {
               arm1_servo.setTargetAngle(i+1,*data);
                data++;
    }
}
std_msgs__msg__Int16MultiArray Arm1_getCurrentAngle(void)
{
    std_msgs__msg__Int16MultiArray arm_servo_states_data;
    for (int i = 0; i < 6; i++) {
        arm_servo_states_data.data.data[i]=arm1_servo.getCurrentAngle(i+1);
    }

    return arm_servo_states_data;
}
#ifdef __cplusplus
}
#endif