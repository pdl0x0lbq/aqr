//
// Created by sw on 24-1-31.
//

#ifndef MICROROS_SERVO_H
#define MICROROS_SERVO_H
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "cmsis_os.h"
extern QueueHandle_t  DDSM115_Servo_DataQueue;
union Int16 {
    short full;      // 整个16位整数
    struct {
        unsigned char low;    // 低8位
        unsigned char high;   // 高8位
    } parts;
};
class servo {
private:
    unsigned char id;
    unsigned char crc;
    int16_t target_angle=0;
public:
    float current_angle=0;
    servo(unsigned char identifier);
    float getCurrentAngle();
    bool setTargetAngle(float target);
};


#endif //MICROROS_SERVO_H
