//
// Created by cmy on 22-10-21.
//

#include "util.h"
unsigned short checksum(unsigned char* buf, int size) {
    unsigned int checkSum = 0;
    for (int i = 0; i < size; i += 2) {
        unsigned short first = (unsigned short)buf[i] << 8;
        unsigned short second = (unsigned short)buf[i+1] & 0x00ff;
        checkSum += first + second;
    }

    while (1) {
        unsigned short c = (checkSum >> 16);
        if (c > 0) {
            checkSum = (checkSum << 16) >> 16;
            checkSum += c;
        } else {
            break;
        }
    }

    return ~checkSum;
}