//
// Created by sw on 24-1-15.
//

#ifndef MICROROS_CRC_H
#define MICROROS_CRC_H
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif
unsigned char crc8(const unsigned char *data, int len) ;

#ifdef __cplusplus
}
#endif
union DDSM115 {
    struct {
        int8_t lowByte;    // 低8位
        int8_t highByte;   // 高8位
    };
    int16_t wheelspeed;   // 16位有符号整数
};


#endif //MICROROS_CRC_H
