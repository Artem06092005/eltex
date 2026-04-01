#pragma once

#include <stddef.h>

enum { max_length_link = 100 };

typedef struct Note {
	struct Note* next;
	struct Note* prev;

	size_t quantityLink;
	char secondName[40];
	char firstName[40];
	char email[40];
	char** socialMediaLink;
} Note;

typedef struct ContactList {
	Note* head;
	Note* tail;
	size_t size;
} ContactList;

void initContactList(ContactList* contactList);

void freeContactList(ContactList* contactList);

void showContactList(ContactList* contactList);

int addContact(ContactList* contactList, const char* first_name,
			   const char* second_name, const char* email,
			   const int quantity_link, const char** socialMediaLink);

int rmNote(ContactList* contactList, size_t index);

int showNote(const ContactList* contactList, const size_t index);

int editNote(ContactList* contactList, const size_t index,
			 const char* first_name, const char* second_name, const char* email,
			 const size_t* quantity_link, const char** socialMediaLink);