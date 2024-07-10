//
// Created by sw on 24-1-31.
//
#define SERVO_MOVE_TIME_WRITE 0x01
#define SERVO_POS_READ 0x1c
#include "servo.h"

#include <stdio.h>
#include <stdlib.h>
// 计算Checksum
uint8_t calculateChecksum(const uint8_t* data, size_t size) {
    uint16_t sum = 0;

    // 计算和
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }

    // 取反并截取为一个字节
    uint8_t checksum = ~static_cast<uint8_t>(sum);

    return checksum;
}
bool servo::setTargetAngle(float target) {
    union Int16 angle;
    union Int16 time;
    time.full=abs(current_angle- target)*57.3/0.84*15;
    current_angle=target;
    angle.full=target*57.3/0.24;
    uint8_t Data[]={id,0x07,SERVO_MOVE_TIME_WRITE,angle.parts.low,angle.parts.high,time.parts.low,time.parts.high};
    size_t dataSize = sizeof(Data);
    uint8_t checksum = calculateChecksum(Data, dataSize);
    uint8_t sendData[]={0x55,0x55,id,0x07,SERVO_MOVE_TIME_WRITE,angle.parts.low,angle.parts.high,time.parts.low,time.parts.high,checksum};
    HAL_UART_Transmit(&huart3, sendData, sizeof(sendData), HAL_MAX_DELAY);
    return true;
}

servo::servo(unsigned char identifier) {
    id = identifier;
}

float servo::getCurrentAngle() {
    union Int16 angle;
    uint8_t buff[10];
    uint8_t string_buff[5];
    uint8_t re_crc=0;
    uint8_t Data[]={id,0x03,SERVO_POS_READ};
    size_t dataSize = sizeof(Data);
    uint8_t checksum = calculateChecksum(Data, dataSize);
    uint8_t sendData[]={0x55,0x55,id,0x03,SERVO_POS_READ,checksum};
    HAL_UART_Transmit(&huart3, sendData, sizeof(sendData), HAL_MAX_DELAY);
    uint32_t timer =xTaskGetTickCount();
    while (1) {
        if (pdPASS == xQueueReceive(DDSM115_Servo_DataQueue, buff, 0)) {
            if(buff[0] == 0x55 && buff[1] == 0x55){
                for (int i = 0; i < 5; ++i) {
                    string_buff[i] = buff[2+i];
                }
                re_crc=buff[7];
                size_t re_dataSize = sizeof(string_buff);
                uint8_t re_checksum = calculateChecksum(string_buff, re_dataSize);
                if(re_checksum==re_crc){
                        angle.parts.low=string_buff[3];
                        angle.parts.high=string_buff[4];
                    current_angle=angle.full*0.24*0.01745;
                    return current_angle;
                } else{
                    return current_angle;
                }
            } else {
                return current_angle;
            }
        }else{
            if (10 < (unsigned long)(xTaskGetTickCount() - timer)){
                return current_angle;
            }
        }
    }
}
