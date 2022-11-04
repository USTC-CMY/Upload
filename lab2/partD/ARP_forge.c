//
// Created by cmy on 22-10-20.
//
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/if_ether.h>
#include<netinet/tcp.h>
#include<linux/if_packet.h>
#include<arpa/inet.h>
#include "util.h"
#include "ARP_forge.h"

#pragma pack(1)

extern struct ifreq ifreq_arp, ifreq_ip;
extern int sock_raw;
extern unsigned char *sendbuff;
extern int total_len;

struct arp_bdy {
    unsigned char source_hw_addr[6];
    uint32_t source_ip_addr;
    unsigned char dst_hw_addr[6];
    uint32_t dst_ip_addr;
};

void get_arp() {
    memset(&ifreq_arp, 0, sizeof(ifreq_arp));
    strncpy(ifreq_arp.ifr_name, "ens33", IFNAMSIZ - 1);
    if (ioctl(sock_raw, SIOCGIFADDR, &ifreq_arp) < 0) {
        printf("error in SIOCGIFADDR \n");
    }

    printf("---%s\n", inet_ntoa((((struct sockaddr_in *) &(ifreq_arp.ifr_addr))->sin_addr)));

    struct arphdr *arph = (struct arphdr *) (sendbuff + sizeof(struct ethhdr));
    arph->ar_hrd = htons(1);
    arph->ar_pro = htons(2048);
    arph->ar_hln = 6;
    arph->ar_pln = 4;
    arph->ar_op = htons(1);

    struct arp_bdy *arpbody = (struct arp_bdy *) (sendbuff + sizeof(struct ethhdr) + sizeof(struct arphdr));
    struct ethhdr *eth = (struct ethhdr *) (sendbuff);
    arpbody->source_hw_addr[0] = eth->h_source[0];
    arpbody->source_hw_addr[1] = eth->h_source[1];
    arpbody->source_hw_addr[2] = eth->h_source[2];
    arpbody->source_hw_addr[3] = eth->h_source[3];
    arpbody->source_hw_addr[4] = eth->h_source[4];
    arpbody->source_hw_addr[5] = eth->h_source[5];

    arpbody->source_ip_addr = inet_addr(inet_ntoa((((struct sockaddr_in *) &(ifreq_arp.ifr_addr))->sin_addr)));

    arpbody->dst_hw_addr[0] = 0x00;
    arpbody->dst_hw_addr[1] = 0x00;
    arpbody->dst_hw_addr[2] = 0x00;
    arpbody->dst_hw_addr[3] = 0x00;
    arpbody->dst_hw_addr[4] = 0x00;
    arpbody->dst_hw_addr[5] = 0x00;

    arpbody->dst_ip_addr = inet_addr("192.168.75.1");

}