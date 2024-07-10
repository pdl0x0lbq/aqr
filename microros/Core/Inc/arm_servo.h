//
// Created by sw on 24-3-27.
//

#ifndef MICROROS_ARM_SERVO_H
#define MICROROS_ARM_SERVO_H
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "cmsis_os.h"
extern QueueHandle_t  Arm_Servo_DataQueue;
union Int16 {
    short full;      // 整个16位整数
    struct {
        unsigned char low;    // 低8位
        unsigned char high;   // 高8位
    } parts;
};
class arm_servo {
private:
    unsigned char id;
    UART_HandleTypeDef huartx;
    unsigned char crc;
    int16_t target_angle=0;
    float current_angle=0;
public:
    arm_servo(unsigned char identifier,UART_HandleTypeDef *huart);
    float getCurrentAngle(unsigned char identifier);
    bool setTargetAngle(unsigned char identifier,float target);
};



#endif //MICROROS_ARM_SERVO_H
