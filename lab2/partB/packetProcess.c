#include <stdio.h>
#include <arpa/inet.h>
#include "packetProcess.h"

void packet_print(unsigned char* buffer, int buffer_len){
    printf("\n---------------------------- Packet ----------------------------\n");

    /*
     * Todo("Exercise 1: complete the packet_print function.")
     */

    int i=0;
    for(i=0;i<buffer_len;i++){
        if(i != 0 && i%16 == 0 )
            printf("\n");
        printf(" %.2X ",buffer[i]);

    }

    printf("\n----------------------- Packet finished ------------------------");
}
