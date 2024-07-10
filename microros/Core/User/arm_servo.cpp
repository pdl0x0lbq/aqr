//
// Created by sw on 24-3-27.
//

#include "arm_servo.h"
#define SERVO_MOVE_TIME_WRITE 0x01
#define SERVO_POS_READ 0x1c
#include "arm_servo.h"
// 计算Checksum
uint8_t calculateChecksum1(const uint8_t* data, size_t size) {
    uint16_t sum = 0;

    // 计算和
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }

    // 取反并截取为一个字节
    uint8_t checksum = ~static_cast<uint8_t>(sum);

    return checksum;
}
bool arm_servo::setTargetAngle(unsigned char identifier,float target) {
    id=identifier;
    union Int16 angle;
    angle.full=target*57.3/0.24;
    uint8_t Data[]={id,0x07,SERVO_MOVE_TIME_WRITE,angle.parts.low,angle.parts.high,0x2c,0x01};
    size_t dataSize = sizeof(Data);
    uint8_t checksum = calculateChecksum1(Data, dataSize);
    uint8_t sendData[]={0x55,0x55,id,0x07,SERVO_MOVE_TIME_WRITE,angle.parts.low,angle.parts.high,0x2c,0x01,checksum};
    HAL_UART_Transmit(&huartx, sendData, sizeof(sendData), 0xffff);
    return true;
}
arm_servo::arm_servo(unsigned char identifier,UART_HandleTypeDef *huart) {
    id = identifier;
    huartx=*huart;
}
float arm_servo::getCurrentAngle(unsigned char identifier) {
    id=identifier;
    union Int16 angle;
    uint8_t buff[8];
    uint8_t string_buff[5];
    uint8_t re_crc=0;
    uint8_t Data[]={id,0x03,SERVO_POS_READ};
    size_t dataSize = sizeof(Data);
    uint8_t checksum = calculateChecksum1(Data, dataSize);
    uint8_t sendData[]={0x55,0x55,id,0x03,SERVO_POS_READ,checksum};
    HAL_UART_Transmit(&huartx, sendData, sizeof(sendData), 0xffff);
    uint32_t timer =xTaskGetTickCount();
    //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
    while (1) {
        if (pdPASS == xQueueReceive(Arm_Servo_DataQueue, buff, 0)) {

            if(buff[0] == 0x55 && buff[1] == 0x55){

                for (int i = 0; i < 5; ++i) {
                    string_buff[i] = buff[2+i];

                }
                re_crc=buff[7];
                size_t re_dataSize = sizeof(string_buff);
                uint8_t re_checksum = calculateChecksum1(string_buff, re_dataSize);

                if(re_checksum==re_crc){

                    //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
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



            //HAL_UART_Transmit(&huart3, buff, sizeof(buff), 0xffff);


        }else{
            if (100 < (unsigned long)(xTaskGetTickCount() - timer)){

                //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
                return current_angle;

            }
        }

    }
    return 0;
}
