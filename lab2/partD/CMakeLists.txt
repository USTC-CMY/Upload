cmake_minimum_required(VERSION 3.23)
project(partD C)

set(CMAKE_C_STANDARD 11)

add_executable(partD
        TCP_forge.c TCP_forge.h ARP_forge.c ARP_forge.h ICMP_forge.c ICMP_forge.h main.c Sample_forge_udp.c util.c util.h)
