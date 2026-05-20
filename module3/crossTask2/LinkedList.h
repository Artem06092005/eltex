#pragma once

#include <sys/types.h>

typedef struct {
	pid_t pid;
} Data;

typedef struct Node {
	Data data;
	struct Node* next;
} Node;

typedef struct {
	Node* head;
} LinkedList;

LinkedList* initLinkedList();
Node* addDriver(LinkedList* lst, pid_t pid);
Node* findDriver(LinkedList* lst, pid_t pid);
void freeLinkedList(LinkedList* lst);