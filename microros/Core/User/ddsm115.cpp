//
// Created by sw on 24-1-30.
//

#include "ddsm115.h"


#include "crc.h"

bool ddsm115::setTargetSpeed(int16_t target) {
    union DDSM115 ddsm115{};
    union DDSM115 re_ddsm115{};
    ddsm115.wheelspeed=target;
    uint8_t buff[10];
    uint8_t string_buff[9];
    uint8_t re_crc=0;
    uint8_t re_crc_result=0;
    unsigned char Data[] = {id, 0x64, static_cast<unsigned char>(ddsm115.highByte), static_cast<unsigned char>(ddsm115.lowByte), 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t data_size=sizeof(Data) / sizeof(Data[0]);
    uint8_t crc_result = crc8(Data, data_size);
    unsigned char sendData[] = {id, 0x64, static_cast<unsigned char>(ddsm115.highByte), static_cast<unsigned char>(ddsm115.lowByte),0x00, 0x00, 0x00, 0x00, 0x00, crc_result};
    HAL_UART_Transmit(&huart1, sendData, sizeof(sendData), HAL_MAX_DELAY);
    uint32_t timer =xTaskGetTickCount();
    while(1){
    if(pdPASS== xQueueReceive(R485_DataQueue, buff, 0)){
        for (int i = 0; i < 9; ++i) {
            string_buff[i] = buff[i];
        }
        re_crc=buff[9];
        size_t re_data_size=sizeof(string_buff) / sizeof(string_buff[0]);
        re_crc_result = crc8(string_buff, re_data_size);
        if (re_crc_result==re_crc){

            re_ddsm115.highByte=string_buff[4];
            re_ddsm115.lowByte=string_buff[5];
            current_speed=re_ddsm115.wheelspeed;
            return true;
        } else{
            return false;
        }
    }else{
        if (100 < (unsigned long)(xTaskGetTickCount() - timer)){
            return false;
        }
    }
    }
}

ddsm115::ddsm115(unsigned char identifier,unsigned char query_crc) {
    crc=query_crc;
    id = identifier;
}

int16_t ddsm115::getCurrentSpeed() {
    uint8_t buff[10];
    uint8_t string_buff[9];
    uint8_t re_crc=0;
    uint8_t re_crc_result=0;
    union DDSM115 re_ddsm115{};
    unsigned char sendData[] = {id, 0x74, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,crc};
    HAL_UART_Transmit(&huart1, sendData, sizeof(sendData), HAL_MAX_DELAY);
    uint32_t timer =xTaskGetTickCount();
    while(1){
        if(pdPASS== xQueueReceive(R485_DataQueue, buff, 0)){
            for (int i = 0; i < 9; ++i) {
                string_buff[i] = buff[i];
            }
            re_crc=buff[9];
            size_t re_data_size=sizeof(string_buff) / sizeof(string_buff[0]);
            re_crc_result = crc8(string_buff, re_data_size);
            if (re_crc_result==re_crc){

                re_ddsm115.highByte=string_buff[4];
                re_ddsm115.lowByte=string_buff[5];
                current_speed=re_ddsm115.wheelspeed;
                return current_speed;

            } else{
                return current_speed;
            }
        }else{
            if (10 < (unsigned long)(xTaskGetTickCount() - timer)){
                return current_speed;
            }
        }
    }
}


