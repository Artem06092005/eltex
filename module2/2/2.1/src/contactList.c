#include "contactList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t counter = 0;

int initContactList(ContactList *contactList, size_t size) {
	contactList->quantityNotes = 0;
	contactList->size = size;
	contactList->notes = malloc(sizeof(Note) * size);
	if (contactList->notes == NULL) return -1;
	contactList->notes->id = counter++;
	contactList->notes->quantityLink = 0;
	return 0;
}

void freeContactList(ContactList *contactList) {
	for (int i = 0; i < contactList->quantityNotes; i++) {
		for (int j = 0; j < contactList->notes[i].quantityLink; j++) {
			free(contactList->notes[i].socialMediaLink[j]);
		}
	}

	free(contactList->notes);
}

void showContactList(ContactList *contactList) {
	if (contactList->quantityNotes == 0) {
		printf("Список контактов пуст\n");
		return;
	}
	for (int i = 0; i < contactList->quantityNotes; i++) {
		printf("Id: %lu ; Name: %s ; Second name: %s\n",
			   contactList->notes[i].id, contactList->notes[i].firstName,
			   contactList->notes[i].secondName);
	}
}

static int reallocContactList(ContactList *contactList) {
	Note *temp =
		realloc(contactList->notes, sizeof(Note) * contactList->size * 2);
	if (temp == NULL) {
		return -1;
	}
	contactList->notes = temp;
	contactList->size *= 2;
	return 0;
}

int addContact(ContactList *contactList, const char *first_name,
			   const char *second_name, const char *email,
			   const size_t quantity_link, const char **social_media_link) {
	if (contactList->size <= contactList->quantityNotes) {
		if (reallocContactList(contactList) == -1) return -1;
	}

	Note *curr_note = &contactList->notes[contactList->quantityNotes];

	if (first_name != NULL) {
		strncpy(curr_note->firstName, first_name,
				sizeof(contactList->notes->firstName) - 1);
	} else {
		curr_note->firstName[0] = '\0';
	}

	if (second_name != NULL) {
		strncpy(curr_note->secondName, second_name,
				sizeof(contactList->notes->secondName) - 1);
	} else {
		curr_note->secondName[0] = '\0';
	}

	if (email != NULL) {
		strncpy(curr_note->email, email, sizeof(contactList->notes->email) - 1);
	} else {
		curr_note->email[0] = '\0';
	}
	curr_note->quantityLink = quantity_link;

	if (social_media_link != NULL && quantity_link > 0) {
		curr_note->socialMediaLink = malloc(sizeof(char *) * quantity_link);
		for (int i = 0; i < quantity_link; i++) {
			curr_note->socialMediaLink[i] =
				malloc(sizeof(char) * max_length_link);
			if (social_media_link[i] != NULL) {
				strncpy(curr_note->socialMediaLink[i], social_media_link[i],
						sizeof(char) * max_length_link - 1);
			}
		}
	}
	curr_note->id = counter++;
	contactList->quantityNotes++;
	return 0;
}

int addContactFromNote(ContactList *contactList, Note *note) {
	if (contactList->size <= contactList->quantityNotes) {
		if (reallocContactList(contactList) == -1) return -1;
	}

	Note *curr_note = &contactList->notes[contactList->quantityNotes];

	if (note->firstName != NULL) {
		strncpy(curr_note->firstName, note->firstName,
				sizeof(contactList->notes->firstName) - 1);
	} else {
		curr_note->firstName[0] = '\0';
	}

	if (note->secondName != NULL) {
		strncpy(curr_note->secondName, note->secondName,
				sizeof(contactList->notes->secondName) - 1);
	} else {
		curr_note->secondName[0] = '\0';
	}

	if (note->email != NULL) {
		strncpy(curr_note->email, note->email,
				sizeof(contactList->notes->email) - 1);
	} else {
		curr_note->email[0] = '\0';
	}
	curr_note->quantityLink = note->quantityLink;

	if (note->quantityLink > 0 && note->socialMediaLink != NULL) {
		curr_note->socialMediaLink =
			malloc(sizeof(char *) * note->quantityLink);
		for (int i = 0; i < note->quantityLink; i++) {
			curr_note->socialMediaLink[i] =
				malloc(sizeof(char) * max_length_link);
			if (note->socialMediaLink[i] != NULL) {
				strncpy(curr_note->socialMediaLink[i], note->socialMediaLink[i],
						sizeof(char) * max_length_link - 1);
			}
		}
	}
	curr_note->id = counter++;
	contactList->quantityNotes++;
	return 0;
}

int initSocialMediaLink(Note *note, size_t l) {
	note->socialMediaLink = malloc(sizeof(char *) * l);
	for (int i = 0; i < l; i++) {
		note->socialMediaLink[i] = malloc(max_length_link * sizeof(char));
		if (note->socialMediaLink[i] == NULL) return -1;
	}
	return 0;
}

int findIndexById(const ContactList *contactList, const size_t id) {
	int left = 0, right = contactList->quantityNotes - 1;
	int mid;

	while (left <= right) {
		mid = (left + right) / 2;
		if (contactList->notes[mid].id == id) return mid;
		if (contactList->notes[mid].id < id) {
			left = mid + 1;
		} else if (contactList->notes[mid].id > id) {
			right = mid - 1;
		}
	}
	return -1;
}

int rmNote(ContactList *contactList, size_t id) {
	int index = findIndexById(contactList, id);
	if (index == -1) return -1;

	for (int i = 0; i < contactList->notes[index].quantityLink; i++) {
		free(contactList->notes[index].socialMediaLink[i]);
	}
	free(contactList->notes[index].socialMediaLink);

	for (int i = index; i < contactList->quantityNotes - 1; i++) {
		contactList->notes[i] = contactList->notes[i + 1];
	}
	contactList->quantityNotes--;
	return 0;
}

int showNote(const ContactList *contactList, const size_t id) {
	int index = findIndexById(contactList, id);
	if (index == -1) return -1;

	const Note *curr_cont = &contactList->notes[index];
	printf("ID :  %lu\n", curr_cont->id);
	printf("Name : %s\n", curr_cont->firstName);
	printf("SecondName : %s\n", curr_cont->secondName);
	printf("Email : %s\n", curr_cont->email);
	printf("Quantity link :  %lu\n", curr_cont->quantityLink);

	for (size_t i = 0; i < curr_cont->quantityLink; i++) {
		printf("\tlink %lu: %s\n", i + 1, curr_cont->socialMediaLink[i]);
	}
	return 0;
}

int editNote(ContactList *contactList, const size_t id, const char *first_name,
			 const char *second_name, const char *email,
			 const size_t *quantity_link, const char **socialMediaLink) {
	int index = findIndexById(contactList, id);
	if (index == -1) return -1;
	Note *currNote = &contactList->notes[index];

	if (first_name && strlen(first_name) > 0) {
		strncpy(currNote->firstName, first_name,
				sizeof(currNote->firstName) - 1);
		currNote->firstName[sizeof(currNote->firstName) - 1] = '\0';
	}
	if (second_name && strlen(second_name) > 0) {
		strncpy(currNote->secondName, second_name,
				sizeof(currNote->secondName) - 1);
		currNote->secondName[sizeof(currNote->secondName) - 1] = '\0';
	}
	if (email && strlen(email) > 0) {
		strncpy(currNote->email, email, sizeof(currNote->email) - 1);
		currNote->email[sizeof(currNote->email) - 1] = '\0';
	}
	if (quantity_link && socialMediaLink && *quantity_link > 0) {
		for (size_t i = 0; i < currNote->quantityLink; i++) {
			free(currNote->socialMediaLink[i]);
		}
		free(currNote->socialMediaLink);

		currNote->quantityLink = *quantity_link;
		currNote->socialMediaLink =
			malloc(sizeof(char *) * currNote->quantityLink);
		if (currNote->socialMediaLink == NULL) return -1;

		for (size_t i = 0; i < currNote->quantityLink; i++) {
			currNote->socialMediaLink[i] = malloc(max_length_link);
			if (currNote->socialMediaLink[i] == NULL) return -1;
			if (socialMediaLink[i] && strlen(socialMediaLink[i]) > 0) {
				strncpy(currNote->socialMediaLink[i], socialMediaLink[i],
						max_length_link - 1);
				currNote->socialMediaLink[i][max_length_link - 1] = '\0';
			} else {
				currNote->socialMediaLink[i][0] = '\0';
			}
		}
	}
	return 0;
}