#include "pq.h"
#include "packet.h"

#include <stdio.h>
#include <stdlib.h>

priority_queue* create_pq(int length) {
	priority_queue* pq = malloc(sizeof(priority_queue));

	if (!pq) {
		printf("Out of memory.\n");
		return NULL;
	}

	pq->array = malloc(sizeof(packet) * length);

	if (!pq->array) {
		printf("Out of memory.\n");
		free(pq);
		return NULL;
	}

	pq->length = length;
	pq->heap_size = 0;

	return pq;
}

void free_heap(priority_queue* pq) {
	if (!pq) return;

	free(pq->array);
	free(pq);
}

int heap_is_empty(priority_queue* pq) {
	if (!pq) return 1;
	return pq->heap_size <= 0;
}

int heap_parent(int i) {
	return (i - 1) / 2;
}

int heap_left(int i) {
	return 2 * i + 1;
}

int heap_right(int i) {
	return 2 * i + 2;
}

void heap_swap(priority_queue* pq, int a, int b) {
	if (!pq) return;
	packet temp = pq->array[a];
	pq->array[a] = pq->array[b];
	pq->array[b] = temp;
}

void heap_max_heapify(priority_queue* pq, int i) {
	if (!pq) return;

	int l = heap_left(i);
	int r = heap_right(i);
	int largest = 0;

	if (l <= pq->heap_size && pq->array[l].priority > pq->array[i].priority)
		largest = l;
	else largest = i;

	if (r <= pq->heap_size && pq->array[r].priority > pq->array[largest].priority)
		largest = r;

	if (largest != i) {
		heap_swap(pq, i, largest);
		heap_max_heapify(pq, largest);
	}
}

packet peek(priority_queue* pq) {
	packet empty = { 0 };
	if (!pq || heap_is_empty(pq)) return empty;
	return pq->array[0];
}

packet dequeue(priority_queue* pq) {
	packet empty = { 0 };
	if (!pq || heap_is_empty(pq)) return empty;

	packet max = pq->array[0];
	pq->array[0] = pq->array[pq->heap_size - 1];
	pq->heap_size--;
	heap_max_heapify(pq, 0);
	return max;
}

void enqueue(priority_queue* pq, packet key) {
	if (!pq) return;

	pq->heap_size++;
	int i = pq->heap_size - 1;

	while (i > 0 && pq->array[heap_parent(i)].priority < key.priority) {
		pq->array[i] = pq->array[heap_parent(i)];
		i = heap_parent(i);
	}

	pq->array[i] = key;
}