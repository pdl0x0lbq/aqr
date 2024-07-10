//
// Created by sw on 24-1-15.
//
#include "crc.h"
#include "cmsis_os.h"
#include "usart.h"

unsigned char crc8(const unsigned char *data, int len) {
    unsigned char crc = 0x00;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x01) {
                crc = (crc >> 1) ^ 0x8C;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
