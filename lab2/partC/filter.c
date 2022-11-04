#include <stdbool.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <bits/stdint-uintn.h>
#include "filter.h"

bool filterByIpAddress(unsigned char *buffer, char *ip_addr, int type){
    struct sockaddr_in source, dest;
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    // if the type of packet is not IP.
    if((int)ntohs(eth->h_proto) != 2048){
        return false;
    }

    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;

    char *source_ip;
    char *dest_ip;
    char temp[20];
    source_ip=inet_ntoa(source.sin_addr);
    strcpy(temp,source_ip);
    dest_ip=inet_ntoa(dest.sin_addr);
    if(type == 0){          // filter by source address
        if (!strcmp(temp,ip_addr)){
            return true;
        }else{
            return false;
        }
    }else{                  // filter by distination address
        if(!strcmp(dest_ip,ip_addr)){
            return true;
        }else{
            return false;
        }
    }
}

bool filterByMacAddress(unsigned char *buffer, char *mac_addr, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    char temp_source[18];
    char temp_dest[18];
    snprintf(temp_source, 18, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
            eth->h_source[0], eth->h_source[1], eth->h_source[2],
            eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    snprintf(temp_dest, 18, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
            eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
            eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    if(type == 1){          // filter by source address
        if (!strcmp(temp_source,mac_addr)){
            return true;
        }else{
            return false;
        }
    }else{                  // filter by distination address
        if(!strcmp(temp_dest,mac_addr)){
            return true;
        }else{
            return false;
        }
    }


}

bool filterByProtocol(unsigned char* buffer, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    switch((int) ntohs(eth->h_proto)){
        case 2048:{  // 0800 -> ip
            struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
            switch(ip->protocol){
                case 1:                                     // ICMP packet
                    if(type == 2)
                        return true;
                    break;


                case 6:                                     // TCP packet
                    if(type == 3)
                        return true;
                    break;

                case 17:                                    // UDP packet
                    if(type == 4)
                        return true;
                    break;

                default:
                    return false;
            }
            return false;
        }

        case 2054:  // 0806 -> arp
            if(type == 1)
                return true;
            break;

        default:
            return false;
    }
    return false;
}

bool filterByPort(unsigned char *buffer, unsigned short port, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    // if the type of packet is not IP.
    if((int)ntohs(eth->h_proto) != 2048){
        return false;
    }

    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    struct tcphdr *tcp = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    struct udphdr *udp = (struct udphdr*)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    uint16_t sport, dport;
    switch(ip->protocol){
        case 6:                                     // TCP packet
              sport = tcp->th_sport;
              dport = tcp->th_dport;
              if(type == 0 && (unsigned short)sport == port){
                  return true;
              }else if(type == 1 && (unsigned short)dport == port){
                  return true;
              }else{
                  return false;
              }

        case 17:                                    // UDP packet
            sport = udp->uh_sport;
            dport = udp->uh_dport;
            if(type == 0 && (unsigned short)sport == port){
                return true;
            }else if(type == 1 && (unsigned short)dport == port){
                return true;
            }else{
                return false;
            }

        default:
            return false;
    }

}
