#include <stdio.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>
#include "packetProcess.h"


#define BLSWITCH16(A)   (  ( ( (uint16_t)(A) & 0xff00 ) >> 8  )    | \
                           ( ( (uint16_t)(A) & 0x00ff ) << 8  )     )
struct sockaddr_in source, dest;
int total = 0, tcp = 0, udp = 0, icmp = 0, igmp = 0, other = 0, arp_packet = 0;
int ipheader_len;

// print format of etherNet-header
void mac_header(unsigned char *buffer){
    struct ethhdr *eth = (struct ethhdr*)(buffer);
    printf("\nEthernet Header\n");
    printf("\t|-Source Address      : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("\t|-Destination Address : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("\t|-Protocol            : %d\n", ntohs(eth->h_proto));
}

// print format of ip-header
void ip_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct iphdr *ip = (struct iphdr*)(buffer);
    printf("\nIP Header\n");
    printf("\t|-Version                  : %u\n", ip->version);
    printf("\t|-Internet Header Length   : %u DWORDS or %u Bytes\n", ip->ihl, 4 * ip->ihl);
    printf("\t|-Type of Service          : %u\n", ip->tos);
    printf("\t|-Total length             : %u Bytes\n", BLSWITCH16(ip->tot_len));
    printf("\t|-Identification           : %u\n", BLSWITCH16(ip->id));
    printf("\t|-TTL                      : %u\n", ip->ttl);
    printf("\t|-Protocol                 : %u\n", ip->protocol);
    printf("\t|-Header Checksum          : %u\n", BLSWITCH16(ip->check));
    struct in_addr addr;
    addr.s_addr = ip->saddr;
    printf("\t|-Source IP                : %s\n", inet_ntoa(addr));
    addr.s_addr = ip->daddr;
    printf("\t|-Destination IP           : %s\n", inet_ntoa(addr));
}

// print format of arp-header
void arp_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct arphdr *arp = (struct arphdr*)(buffer);
    printf("\nARP Header\n");
    printf("\t|-Hardware Type                  : %u\n", arp->ar_hrd);
    printf("\t|-Protocol Type                  : %u\n", arp->ar_pro);
    printf("\t|-Hardware Size                  : %u\n", arp->ar_hln);
    printf("\t|-Protocol Size                  : %u\n", arp->ar_pln);
    printf("\t|-Opcode                         : %u\n", arp->ar_op);

}

// print format of tcp-header
void tcp_header(unsigned char* buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct tcphdr *tcp = (struct tcphdr*)(buffer);
    printf("\nTCP Header\n");
    printf("\t|-Source Port                  : %u\n", BLSWITCH16(tcp->th_sport));
    printf("\t|-Destination Port             : %u\n", BLSWITCH16(tcp->th_dport));
    printf("\t|-Sequence Number              : %u\n", tcp->th_seq);//uint32_t
    printf("\t|-Acknowledgement Number       : %u\n", tcp->th_ack);//uint32_t
    printf("\t|-Header Length                : %u\n", tcp->th_off * 4);
    printf("\t|-Flags                        : 0x%.2x\n", tcp->th_flags);
    printf("\t|-Window                       : %u\n", BLSWITCH16(tcp->th_win));
    printf("\t|-Checksum                     : %u\n", BLSWITCH16(tcp->th_sum));
    printf("\t|-Urgent Pointer               : %u\n", BLSWITCH16(tcp->th_urp));

}

// print format of udp-header
void udp_header(unsigned char* buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct udphdr *udp = (struct udphdr*)(buffer);
    printf("\nUDP Header\n");
    printf("\t|-Source Port                  : %u\n", BLSWITCH16(udp->uh_sport));
    printf("\t|-Destination Port             : %u\n", BLSWITCH16(udp->uh_dport));
    printf("\t|-UDP Length                   : %u\n", BLSWITCH16(udp->uh_ulen));
    printf("\t|-Checksum                     : %u\n", BLSWITCH16(udp->uh_sum));

}

// print format of icmp-header
void icmp_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct icmphdr *icmp = (struct icmphdr*)(buffer);
    printf("\nICMP Header\n");
    printf("\t|-TYPE                     : %u\n", icmp->type);
    printf("\t|-CODE                     : %u\n", icmp->code);
    printf("\t|-CHECKSUM                 : %u\n", BLSWITCH16(icmp->checksum));
}

// print format of payload
void payload(unsigned char* buffer,int buffer_len){

    printf("\nData\n");
    for(int i=0; i<buffer_len; i++){
        if(i!=0 && i%16==0)
            printf("\n");
        printf(" %.2X ",buffer[i]);
    }

    printf("\n");
}

// print ICMP packet
void ICMP_printer(unsigned char *buffer, int buffer_len){
    printf("\n---------------------------ICMP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer + sizeof(struct ethhdr));
    icmp_header(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    // print packet payload
    unsigned char *data = (buffer + sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct icmphdr));
    int data_len = buffer_len - (sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct icmphdr));
    payload(data, data_len);
    printf("--------------------------------------------------------------\n\n\n");
}

// print ARP packet
void ARP_printer(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    printf("\n---------------------------ARP Packet---------------------------");
    // print header
    mac_header(buffer);
    arp_header(buffer + sizeof(struct ethhdr));
    printf("--------------------------------------------------------------\n\n\n");
}

// print TCP packet
void TCP_printer(unsigned char *buffer, int buffer_len){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    printf("\n---------------------------TCP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer + sizeof(struct ethhdr));
    tcp_header(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    // print packet payload
    unsigned char *data = (buffer + sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct tcphdr));
    int data_len = buffer_len - (sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct tcphdr));
    payload(data, data_len);


    printf("--------------------------------------------------------------\n\n\n");
}

// print UDP packet
void UDP_printer(unsigned char* buffer, int buffer_len){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    printf("\n---------------------------UDP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer + sizeof(struct ethhdr));
    udp_header(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    // print packet payload
    unsigned char *data = (buffer + sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct udphdr));
    int data_len = buffer_len - (sizeof(struct iphdr)  + sizeof(struct ethhdr) + sizeof(struct udphdr));
    payload(data, data_len);


    printf("--------------------------------------------------------------\n\n\n");
}

// packet process func
void packet_process(unsigned char* buffer, int buffer_len){
    ++total;
    struct ethhdr *eth = (struct ethhdr*)(buffer);
    switch((int) ntohs(eth->h_proto)){
        case 2048:{  // 0800 -> ip
            struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
            switch(ip->protocol){
                case 1:                                     // ICMP packet
                    ++icmp;
                    ICMP_printer(buffer, buffer_len);
                    break;

                case 2:
                    ++igmp;
                    break;

                case 6:                                     // TCP packet
                    ++tcp;
                    TCP_printer(buffer, buffer_len);         // print packet info
                    break;

                case 17:                                    // UDP packet
                    ++udp;
                    UDP_printer(buffer,buffer_len);          // print packet info
                    break;

                default:
                    ++other;
            }
            break;
        }

        case 2054:  // 0806 -> arp
            ++arp_packet;
            ARP_printer(buffer);            // print ARP packet info
            break;

        default:
            ++other;
    }

    printf("Result: [TCP : %d], [UDP : %d], [ARP : %d], [ICMP : %d], [IGMP : %d], [OTHER : %d], [TOTAL : %d]\n",
           tcp, udp, arp_packet, icmp, igmp, other, total);
}