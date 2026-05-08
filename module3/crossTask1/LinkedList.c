#include "LinkedList.h"

#include <stdlib.h>

LinkedList* initLinkedList() {
	LinkedList* list = malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

static Node* initNode(u_int32_t ip, u_int16_t port) {
	Node* node = malloc(sizeof(Node));
	node->next = NULL;
	node->data.counter = 1;
	node->data.ip = ip;
	node->data.port = port;
	return node;
}

Node* addClient(LinkedList* lst, u_int32_t ip, u_int16_t port) {
	if (lst->head == NULL) {
		lst->head = initNode(ip, port);
		return lst->head;
	} else {
		Node* p = lst->head;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = initNode(ip, port);
		return p->next;
	}
}

Node* findClient(LinkedList* lst, u_int32_t ip, u_int16_t port) {
	Node* p = lst->head;
	while (p != NULL) {
		if (p->data.ip == ip && p->data.port == port) return p;
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