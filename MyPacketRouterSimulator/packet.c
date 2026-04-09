#include "packet.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t parse_ip(char* ip_str) {
    int a, b, c, d;
    sscanf_s(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// format the string we make into the buffer
void stringify_ip(uint32_t ip, char* buff, int buff_size) {
    sprintf_s(buff, buff_size, "%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
}