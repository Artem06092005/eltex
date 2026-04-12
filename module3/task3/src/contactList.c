#include "contactList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initContactList(ContactList* contactList) {
	contactList->head = NULL;
	contactList->tail = NULL;
	contactList->size = 0;
}

void freeContactList(ContactList* contactList) {
	Note* curr = contactList->head;
	while (curr != NULL) {
		Note* next = curr->next;
		free(curr);
		curr = next;
	}
	contactList->head = NULL;
	contactList->tail = NULL;
	contactList->size = 0;
}

void showContactList(ContactList* contactList) {
	if (contactList->size == 0) {
		printf("Список контактов пуст\n");
		return;
	}
	Note* curr = contactList->head;
	for (size_t i = 0; i < contactList->size; i++) {
		printf("Index: %lu ; Name: %s ; Second name: %s\n", i,
			   curr->data.firstName, curr->data.secondName);
		curr = curr->next;
	}
}

static void inputNote(ContactList* contactList, Note* newNote,
					  const char* second_name) {
	Note* nextNote = contactList->head;
	for (size_t i = 0; i < contactList->size; i++) {
		if (strcmp(second_name, nextNote->data.secondName) <= 0) break;
		nextNote = nextNote->next;
	}

	if (contactList->size == 0) {
		contactList->head = newNote;
		contactList->tail = newNote;
		newNote->next = NULL;
		newNote->prev = NULL;

	} else if (nextNote == contactList->head) {
		newNote->next = contactList->head;
		contactList->head->prev = newNote;
		contactList->head = newNote;
		newNote->prev = NULL;
	} else if (nextNote == NULL) {
		newNote->prev = contactList->tail;
		contactList->tail->next = newNote;
		contactList->tail = newNote;
		newNote->next = NULL;
	} else {
		newNote->next = nextNote;
		newNote->prev = nextNote->prev;

		nextNote->prev->next = newNote;
		nextNote->prev = newNote;
	}
}

int addContact(ContactList* contactList, const char* first_name,
			   const char* second_name, const char* email) {
	Note* newNote = malloc(sizeof(Note));

	if (!newNote) return -1;

	if (first_name && strlen(first_name) > 0) {
		strncpy(newNote->data.firstName, first_name,
				sizeof(newNote->data.firstName) - 1);
	}
	if (second_name && strlen(second_name) > 0) {
		strncpy(newNote->data.secondName, second_name,
				sizeof(newNote->data.secondName) - 1);
	}
	if (email && strlen(email) > 0) {
		strncpy(newNote->data.email, email, sizeof(newNote->data.email) - 1);
	}
	inputNote(contactList, newNote, second_name);
	contactList->size++;
	return 0;
}

static Note* fastfindNote(const ContactList* contactList, size_t index) {
	Note* curr;
	if (index < contactList->size / 2) {
		curr = contactList->head;
		for (size_t i = 0; i < index; i++) {
			curr = curr->next;
		}
	} else {
		curr = contactList->tail;
		for (size_t i = contactList->size - 1; i > index; i--) {
			curr = curr->prev;
		}
	}
	return curr;
}

int rmNote(ContactList* contactList, size_t index) {
	Note* toDel;

	if (index >= contactList->size) {
		return -1;
	}
	if (contactList->size == 1) {
		free(contactList->head);
		contactList->head = NULL;
		contactList->tail = NULL;

	} else if (index == 0) {
		toDel = contactList->head;
		contactList->head = contactList->head->next;

		free(toDel);
		contactList->head->prev = NULL;

	} else if (index == contactList->size - 1) {
		toDel = contactList->tail;
		contactList->tail = contactList->tail->prev;

		free(toDel);
		contactList->tail->next = NULL;
	} else {
		toDel = fastfindNote(contactList, index);
		toDel->prev->next = toDel->next;
		toDel->next->prev = toDel->prev;

		free(toDel);
	}
	contactList->size--;
	return 0;
}

int showNote(const ContactList* contactList, const size_t index) {
	if (index >= contactList->size) return -1;

	Note* toShow = fastfindNote(contactList, index);
	printf("Index :  %lu\n", index);
	printf("Name : %s\n", toShow->data.firstName);
	printf("SecondName : %s\n", toShow->data.secondName);
	printf("Email : %s\n", toShow->data.email);
	return 0;
}

int editNote(ContactList* contactList, const size_t index,
			 const char* first_name, const char* second_name,
			 const char* email) {
	if (index >= contactList->size) return -1;

	Note* toEdit = fastfindNote(contactList, index);
	if (!toEdit) return -1;

	if (first_name && strlen(first_name) > 0) {
		strncpy(toEdit->data.firstName, first_name,
				sizeof(toEdit->data.firstName) - 1);
	}
	if (email && strlen(email) > 0) {
		strncpy(toEdit->data.email, email, sizeof(toEdit->data.email) - 1);
	}

	if (second_name && strlen(second_name) > 0) {
		strncpy(toEdit->data.secondName, second_name,
				sizeof(toEdit->data.secondName) - 1);

		if (toEdit->prev) {
			toEdit->prev->next = toEdit->next;
		} else {
			contactList->head = toEdit->next;
		}
		if (toEdit->next) {
			toEdit->next->prev = toEdit->prev;
		} else {
			contactList->tail = toEdit->prev;
		}
		contactList->size--;
		inputNote(contactList, toEdit, second_name);
		contactList->size++;
	}
	return 0;
}