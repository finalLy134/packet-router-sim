#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hashtable* create_hashtable() {
	hashtable* table = malloc(sizeof(hashtable));

	if (!table) {
		printf("Out of memory.\n");
		return NULL;
	}
	for (int i = 0; i < TABLE_SIZE; i++)
		table->arr[i] = NULL;

	return table;
}

void free_hashtable(hashtable* table) {
	if (!table) return;

	for (int i = 0; i < TABLE_SIZE; i++) {
		node* curr = table->arr[i];

		while (curr) {
			node* tmp = curr;
			curr = curr->next;
			free(tmp);
		}
	}

	free(table);
}

int hash_ip(uint32_t ip, int size) {
	return ip % size;
}

void put(hashtable* table, uint32_t ip, char mac[MAC_SIZE]) {
	int index = hash_ip(ip, TABLE_SIZE);

	node* new_node = malloc(sizeof(node));
	if (!new_node) {
		printf("Out of memory\n");
		return;
	}

	new_node->ip = ip;
	strcpy_s(new_node->mac, MAC_SIZE, mac);
	new_node->next = table->arr[index];

	table->arr[index] = new_node;
}

char* get(hashtable* table, uint32_t ip) {
	int index = hash_ip(ip, TABLE_SIZE);
	node* bucket = table->arr[index];
	while (bucket != NULL)
	{
		if (bucket->ip == ip)
		{
			return bucket->mac;
		}
		bucket = bucket->next;
	}
	return NULL;
}