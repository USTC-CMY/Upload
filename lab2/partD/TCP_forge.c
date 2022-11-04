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
#include "TCP_forge.h"
#pragma pack(1)

extern struct ifreq ifreq_tcp;
extern int sock_raw;
extern unsigned char *sendbuff;
extern int total_len;
// function define

void get_tcp_data(){
    struct tcphdr *th = (struct tcphdr *)(sendbuff + sizeof(struct iphdr) + sizeof(struct ethhdr));
    th->th_sport	= htons(23451);
    th->th_dport	= htons(8080);
    th->th_seq	= 0;
    th->th_ack = 0;
    th->th_x2 = 0;
    th->th_off = 6;
    th->th_flags = TH_SYN;
    th->th_win = htons(14156);
    th->th_urp = 0;

    total_len+= sizeof(struct tcphdr);
    sendbuff[total_len++]	=	0x68;
    sendbuff[total_len++]	=	0x65;
    sendbuff[total_len++]	=	0x6C;
    sendbuff[total_len++]	=	0x6C;
    th->th_sum = htons(0x00c3);
}



void get_tcp(){
    memset(&ifreq_tcp,0,sizeof(ifreq_tcp));
    strncpy(ifreq_tcp.ifr_name,"ens33",IFNAMSIZ-1);
    if(ioctl(sock_raw,SIOCGIFADDR,&ifreq_tcp)<0){
        printf("error in SIOCGIFADDR \n");
    }

    printf("%s\n",inet_ntoa((((struct sockaddr_in*)&(ifreq_tcp.ifr_addr))->sin_addr)));

    struct iphdr *iph = (struct iphdr*)(sendbuff + sizeof(struct ethhdr));
    iph->ihl	= 5;
    iph->version	= 4;
    iph->tos	= 0;
    iph->id		= htons(10201);
    iph->ttl	= 64;
    iph->protocol	= 6;
    iph->saddr	= inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_tcp.ifr_addr))->sin_addr)));
    iph->daddr	= inet_addr("192.168.75.1"); // put destination IP address
    printf("destIP:%.2X\n",iph->daddr);
    total_len += sizeof(struct iphdr);
    get_tcp_data();

    iph->tot_len	= htons(total_len - sizeof(struct ethhdr));
    iph->check	= htons(
            checksum((unsigned char*)(sendbuff + sizeof(struct ethhdr)),sizeof(struct iphdr)));
}