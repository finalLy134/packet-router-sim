#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

typedef enum { LOW, HIGH } priority_type;

typedef struct packet {
	uint32_t src;
	uint32_t dest;
	priority_type priority;
	int size;
} packet;

uint32_t parse_ip(char* ip_str);
void stringify_ip(uint32_t ip, char* buff, int buff_size);

#endif
