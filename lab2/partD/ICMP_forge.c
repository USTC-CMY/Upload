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
#include <netinet/ip_icmp.h>
#include "util.h"
#include "ICMP_forge.h"
#pragma pack(1)

extern struct ifreq ifreq_ip;
extern int sock_raw;
extern unsigned char *sendbuff;
extern int total_len;


void get_icmp_data() {
    struct icmphdr *icmp = (struct icmphdr *) (sendbuff + sizeof(struct iphdr) + sizeof(struct ethhdr));
    icmp->type = 8; // ping request
    icmp->code = 0;
    icmp->un.echo.id = htons(1);
    icmp->un.echo.sequence = htons(11111);
    total_len += sizeof(struct icmphdr);
    icmp->checksum = htons(checksum((unsigned char *) (sendbuff + sizeof(struct ethhdr) + sizeof(struct iphdr)),
                                     sizeof(struct icmphdr)));
}

void get_icmp() {
    memset(&ifreq_ip, 0, sizeof(ifreq_ip));
    strncpy(ifreq_ip.ifr_name, "ens33", IFNAMSIZ - 1);
    if (ioctl(sock_raw, SIOCGIFADDR, &ifreq_ip) < 0) {
        printf("error in SIOCGIFADDR \n");
    }

    printf("%s\n", inet_ntoa((((struct sockaddr_in *) &(ifreq_ip.ifr_addr))->sin_addr)));

    struct iphdr *iph = (struct iphdr *) (sendbuff + sizeof(struct ethhdr));
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->id = htons(10201);
    iph->ttl = 64;
    iph->protocol = 1; // ICMP
    iph->saddr = inet_addr(inet_ntoa((((struct sockaddr_in *) &(ifreq_ip.ifr_addr))->sin_addr)));
    iph->daddr = inet_addr("192.168.75.1"); // put destination IP address
    printf("destIP:%.2X\n", iph->daddr);
    total_len += sizeof(struct iphdr);
    get_icmp_data();

    iph->tot_len = htons(total_len - sizeof(struct ethhdr));
    iph->check = htons(
            checksum((unsigned char *) (sendbuff + sizeof(struct ethhdr)), sizeof(struct iphdr)));
}