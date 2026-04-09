#include "packet.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t parse_ip(char* ip_str) {
    // we shift by the amount of bits to fit in the octet
    // afterwards we OR them together to keep all of them (basically concatenate)
    int a, b, c, d;
    sscanf_s(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// format the string we make into the buffer
void stringify_ip(uint32_t ip, char* buff, int buff_size) {
    // we shift right by the octet size to bring it back to a regular byte
    // then we AND with 0xFF to keep only the bits we shifted (the actual number)
    sprintf_s(buff, buff_size, "%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
}
