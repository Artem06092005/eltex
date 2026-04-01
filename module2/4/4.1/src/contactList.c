#include "contactList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initContactList(ContactList* contactList) {
	contactList->head = NULL;
	contactList->tail = NULL;
	contactList->size = 0;
}

static void freeLinks(Note* note) {
	for (size_t i = 0; i < note->quantityLink; i++) {
		free(note->socialMediaLink[i]);
	}
	free(note->socialMediaLink);
}

void freeContactList(ContactList* contactList) {
	Note* curr = contactList->head;
	while (curr != NULL) {
		Note* next = curr->next;
		freeLinks(curr);
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
		printf("Index: %lu ; Name: %s ; Second name: %s\n", i, curr->firstName,
			   curr->secondName);
		curr = curr->next;
	}
}

static void inputNote(ContactList* contactList, Note* newNote,
					  const char* second_name) {
	Note* nextNote = contactList->head;
	for (size_t i = 0; i < contactList->size; i++) {
		if (strcmp(second_name, nextNote->secondName) <= 0) break;
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
			   const char* second_name, const char* email,
			   const int quantity_link, const char** socialMediaLink) {
	Note* newNote = malloc(sizeof(Note));

	if (!newNote) return -1;

	if (first_name && strlen(first_name) > 0) {
		strncpy(newNote->firstName, first_name, sizeof(newNote->firstName) - 1);
	}
	if (second_name && strlen(second_name) > 0) {
		strncpy(newNote->secondName, second_name,
				sizeof(newNote->secondName) - 1);
	}
	if (email && strlen(email) > 0) {
		strncpy(newNote->email, email, sizeof(newNote->email) - 1);
	}
	newNote->quantityLink = quantity_link;

	newNote->socialMediaLink = malloc(sizeof(char*) * quantity_link);
	if (!newNote->socialMediaLink) return -1;

	for (size_t i = 0; i < quantity_link; i++) {
		newNote->socialMediaLink[i] = malloc(max_length_link * sizeof(char));
		if (!newNote->socialMediaLink[i]) return -1;
		strncpy(newNote->socialMediaLink[i], socialMediaLink[i],
				max_length_link - 1);
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
		freeLinks(contactList->head);
		free(contactList->head);
		contactList->head = NULL;
		contactList->tail = NULL;

	} else if (index == 0) {
		toDel = contactList->head;
		contactList->head = contactList->head->next;
		freeLinks(toDel);
		free(toDel);
		contactList->head->prev = NULL;

	} else if (index == contactList->size - 1) {
		toDel = contactList->tail;
		contactList->tail = contactList->tail->prev;
		freeLinks(toDel);
		free(toDel);
		contactList->tail->next = NULL;
	} else {
		toDel = fastfindNote(contactList, index);
		toDel->prev->next = toDel->next;
		toDel->next->prev = toDel->prev;
		freeLinks(toDel);
		free(toDel);
	}
	contactList->size--;
	return 0;
}

int showNote(const ContactList* contactList, const size_t index) {
	if (index >= contactList->size) return -1;

	Note* toShow = fastfindNote(contactList, index);
	printf("Index :  %lu\n", index);
	printf("Name : %s\n", toShow->firstName);
	printf("SecondName : %s\n", toShow->secondName);
	printf("Email : %s\n", toShow->email);
	printf("Quantity link :  %lu\n", toShow->quantityLink);

	for (size_t i = 0; i < toShow->quantityLink; i++) {
		printf("\tlink %lu: %s\n", i + 1, toShow->socialMediaLink[i]);
	}

	return 0;
}

int editNote(ContactList* contactList, const size_t index,
			 const char* first_name, const char* second_name, const char* email,
			 const size_t* quantity_link, const char** socialMediaLink) {
	if (index >= contactList->size) return -1;

	Note* toEdit = fastfindNote(contactList, index);
	if (!toEdit) return -1;

	if (first_name && strlen(first_name) > 0) {
		strncpy(toEdit->firstName, first_name, sizeof(toEdit->firstName) - 1);
	}
	if (email && strlen(email) > 0) {
		strncpy(toEdit->email, email, sizeof(toEdit->email) - 1);
	}
	if (quantity_link && socialMediaLink && *quantity_link > 0) {
		freeLinks(toEdit);

		toEdit->quantityLink = *quantity_link;
		toEdit->socialMediaLink = malloc(sizeof(char*) * toEdit->quantityLink);
		if (!toEdit->socialMediaLink) return -1;

		for (size_t i = 0; i < *quantity_link; i++) {
			toEdit->socialMediaLink[i] = malloc(max_length_link * sizeof(char));
			if (!toEdit->socialMediaLink[i]) return -1;

			strncpy(toEdit->socialMediaLink[i], socialMediaLink[i],
					max_length_link - 1);
		}
	}

	if (second_name && strlen(second_name) > 0) {
		strncpy(toEdit->secondName, second_name,
				sizeof(toEdit->secondName) - 1);

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