#include "queue.h"

#include <stdlib.h>

void initQueue(Queue* queue) { queue->head = NULL; }

static Node* initNode(uint8_t priority, int val) {
	Node* newNode = malloc(sizeof(Node));
	if (!newNode) return NULL;

	newNode->next = NULL;
	newNode->priority = priority;
	newNode->val = val;
	return newNode;
}

int enqueue(Queue* queue, uint8_t priority, int val) {
	Node* newNode = initNode(priority, val);
	if (!newNode) return -1;

	if (queue->head == NULL || priority > queue->head->priority) {
		newNode->next = queue->head;
		queue->head = newNode;
		return 0;
	}
	Node* curr = queue->head;
	while (curr->next != NULL && curr->next->priority >= priority) {
		curr = curr->next;
	}
	newNode->next = curr->next;
	curr->next = newNode;
	return 0;
}

int dequeueFirst(Queue* queue) {
	if (!queue->head) {
		return -1;
	}
	Node* toDel = queue->head;
	queue->head = queue->head->next;
	free(toDel);
	return 0;
}

int dequeueByPriority(Queue* queue, uint8_t priority) {
	if (!queue->head) return -1;

	Node* tmp = queue->head;
	Node* toDel = NULL;
	if (tmp->priority == priority) {
		queue->head = queue->head->next;
		free(tmp);
		return 0;
	}

	while (tmp->next != NULL) {
		if (tmp->next->priority == priority) {
			toDel = tmp->next;
			tmp->next = tmp->next->next;
			free(toDel);
			return 0;
		}
		tmp = tmp->next;
	}
	return -1;
}

int dequeueByMinPriority(Queue* queue, uint8_t priority) {
	if (!queue->head) return -1;

	Node* tmp = queue->head;
	Node* toDel = NULL;
	if (tmp->priority >= priority) {
		queue->head = queue->head->next;
		free(tmp);
		return 0;
	}

	while (tmp->next != NULL) {
		if (tmp->next->priority >= priority) {
			toDel = tmp->next;
			tmp->next = tmp->next->next;
			free(toDel);
			return 0;
		}
		tmp = tmp->next;
	}
	return -1;
}

void freeQueue(Queue* queue) {
	while (queue->head) dequeueFirst(queue);
}