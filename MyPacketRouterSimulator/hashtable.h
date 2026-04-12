#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

#define TABLE_SIZE 257
#define MAC_SIZE 18

typedef struct node {
	uint32_t ip;
	char mac[MAC_SIZE];
	struct node* next;
} node;

typedef struct hashtable {
	node* arr[TABLE_SIZE];
} hashtable;

hashtable* create_hashtable();
void free_hashtable(hashtable* table);

int hash_ip(uint32_t ip, int size);

void put(hashtable* table, uint32_t ip, char mac[MAC_SIZE]);
char* get(hashtable* table, uint32_t ip);

#endif