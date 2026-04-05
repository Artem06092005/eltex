#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct Node {
	uint8_t priority;
	struct Node* next;
	int val;
} Node;

typedef struct Queue {
	Node* head;
} Queue;

void initQueue(Queue* queue);
int enqueue(Queue* queue, uint8_t priority, int val);
int dequeueFirst(Queue* queue);
int dequeueByPriority(Queue* queue, uint8_t priority);
int dequeueByMinPriority(Queue* queue, uint8_t priority);
void freeQueue(Queue* queue);