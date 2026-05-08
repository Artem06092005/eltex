#pragma once

#include <sys/types.h>

typedef struct {
	u_int32_t ip;
	u_int16_t port;
	u_int32_t counter;
} Data;

typedef struct Node {
	Data data;
	struct Node* next;
} Node;

typedef struct {
	Node* head;
} LinkedList;

LinkedList* initLinkedList();
Node* addClient(LinkedList* lst, u_int32_t ip, u_int16_t port);
Node* findClient(LinkedList* lst, u_int32_t ip, u_int16_t port);
void freeLinkedList(LinkedList* lst);