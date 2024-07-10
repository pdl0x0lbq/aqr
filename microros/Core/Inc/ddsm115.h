//
// Created by sw on 24-1-30.
//

#ifndef MICROROS_DDSM115_H
#define MICROROS_DDSM115_H
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ddsm115.h"
#include "usart.h"
#include "armquadrover_msgs//msg/ddsm115.h"
#include "cmsis_os.h"

extern QueueHandle_t  R485_DataQueue;

class ddsm115 {
private:
    unsigned char id;
    unsigned char crc;
    int16_t target_speed=0;
    int16_t current_speed=0;
public:
    ddsm115(unsigned char id,unsigned char query_crc);
    bool setTargetSpeed(int16_t target);

    int16_t getCurrentSpeed();
    void setID(unsigned char identifier) {
        id = identifier;
    }
};


#endif //MICROROS_DDSM115_H
