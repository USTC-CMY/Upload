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
#include <unistd.h>
#include "TCP_forge.h"
#include "ARP_forge.h"
#include "ICMP_forge.h"
// for each ioctl keep diffrent ifreq structure otherwise error may come in sending(sendto )
struct ifreq ifreq_c,ifreq_i,ifreq_ip, ifreq_arp, ifreq_tcp;
int sock_raw;
unsigned char *sendbuff;
int total_len=0,send_len;

#define DESTMAC0	0x00
#define DESTMAC1	0x50
#define DESTMAC2	0x56
#define DESTMAC3	0xc0
#define DESTMAC4	0x00
#define DESTMAC5	0x08

#define destination_ip 192.168.75.1

void get_eth_index();
void get_mac(int type);


int main(){
    sock_raw=socket(AF_PACKET,SOCK_RAW,ETH_P_IP);
    if(sock_raw == -1)
        printf("error in socket");

    sendbuff=(unsigned char*)malloc(64); // increase in case of large data.
    memset(sendbuff,0,64);

    printf("选择协议：1.TCP 2.ARP 3.ICMP\n");
    int type;
    scanf("%d", &type);

    if (type == 1) {
        get_eth_index();
        get_mac(type);
        get_tcp();
    } else if (type == 2) {
        get_eth_index();
        get_mac(type);
        get_arp();
    } else if (type == 3) {
        get_eth_index();
        get_mac(type);
        get_icmp();
    }

    struct sockaddr_ll sadr_ll;
    sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex;
    sadr_ll.sll_halen = ETH_ALEN;
    sadr_ll.sll_addr[0] = DESTMAC0;
    sadr_ll.sll_addr[1] = DESTMAC1;
    sadr_ll.sll_addr[2] = DESTMAC2;
    sadr_ll.sll_addr[3] = DESTMAC3;
    sadr_ll.sll_addr[4] = DESTMAC4;
    sadr_ll.sll_addr[5] = DESTMAC5;

    printf("sending...\n");
    int i = 0;
    while (1) {
        send_len = sendto(sock_raw, sendbuff, 64, 0, (const struct sockaddr *) &sadr_ll, sizeof(struct sockaddr_ll));
        sleep(1);
        if (send_len < 0) {
            printf("error in sending....sendlen=%d....errno=%d\n", send_len, errno);
            return -1;
        }
        if (++i > 10) {
            break;
        }
    }
    return 0;
}

// function definition
void get_eth_index(){
    memset(&ifreq_i,0,sizeof(ifreq_i));
    strncpy(ifreq_i.ifr_name,"ens33",IFNAMSIZ-1);

    if((ioctl(sock_raw,SIOCGIFINDEX,&ifreq_i))<0)
        printf("error in index ioctl reading");

    printf("index=%d\n",ifreq_i.ifr_ifindex);
}

void get_mac(int type){
    memset(&ifreq_c,0,sizeof(ifreq_c));
    strncpy(ifreq_c.ifr_name,"ens33",IFNAMSIZ-1);

    if((ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c))<0)
        printf("error in SIOCGIFHWADDR ioctl reading");

    printf("Mac= %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]),
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]),
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]),
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]),
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]),
           (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]));

    printf("ethernet packaging start ... \n");

    struct ethhdr *eth = (struct ethhdr *)(sendbuff);
    eth->h_source[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
    eth->h_source[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
    eth->h_source[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
    eth->h_source[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
    eth->h_source[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
    eth->h_source[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);

    if (type == 1 || type == 3) {
        eth->h_dest[0] = DESTMAC0;
        eth->h_dest[1] = DESTMAC1;
        eth->h_dest[2] = DESTMAC2;
        eth->h_dest[3] = DESTMAC3;
        eth->h_dest[4] = DESTMAC4;
        eth->h_dest[5] = DESTMAC5;

        eth->h_proto = htons(ETH_P_IP);   //0x0800
    }else{
        eth->h_dest[0] = 0xff;
        eth->h_dest[1] = 0xff;
        eth->h_dest[2] = 0xff;
        eth->h_dest[3] = 0xff;
        eth->h_dest[4] = 0xff;
        eth->h_dest[5] = 0xff;

        eth->h_proto = htons(ETH_P_ARP);
    }
    printf("ethernet packaging done.\n");

    total_len+=sizeof(struct ethhdr);
}

