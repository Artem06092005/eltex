#include "LinkedList.h"

#include <stdlib.h>

LinkedList* initLinkedList() {
	LinkedList* list = malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

static Node* initNode(pid_t pid) {
	Node* node = malloc(sizeof(Node));
	node->next = NULL;
	node->data.pid = pid;
	return node;
}

Node* addDriver(LinkedList* lst, pid_t pid) {
	if (lst->head == NULL) {
		lst->head = initNode(pid);
		return lst->head;
	} else {
		Node* p = lst->head;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = initNode(pid);
		return p->next;
	}
}

Node* findDriver(LinkedList* lst, pid_t pid) {
	Node* p = lst->head;
	while (p != NULL) {
		if (p->data.pid == pid) return p;
		p = p->next;
	}
	return NULL;
}

void freeLinkedList(LinkedList* lst) {
	if (lst == NULL) return;

	Node* p = lst->head;
	while (p != NULL) {
		Node* tmp = p->next;
		free(p);
		p = tmp;
	}
	lst->head = NULL;
	free(lst);
}