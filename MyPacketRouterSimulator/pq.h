#ifndef PQ_H
#define PQ_H

#include "packet.h"

// A priority queue using the heap implementation
typedef struct priority_queue {
	packet* array;
	int heap_size;
	int length;
} priority_queue;

priority_queue* create_pq(int length);
void free_heap(priority_queue* pq);

int heap_is_empty(priority_queue* pq);
int heap_parent(int i);
int heap_left(int i);
int heap_right(int i);
void heap_swap(priority_queue* pq, int a, int b);
void heap_max_heapify(priority_queue* pq, int i);

packet peek(priority_queue* pq);
packet dequeue(priority_queue* pq);
void enqueue(priority_queue* head, packet key);

#endif
