#pragma once

#include <stddef.h>

enum { max_length_link = 100, max_str = 40 };

typedef struct Data {
	char secondName[max_str];
	char firstName[max_str];
	char email[max_str];
} Data;

typedef struct Note {
	struct Note* next;
	struct Note* prev;
	Data data;
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
			   const char* second_name, const char* email);

int rmNote(ContactList* contactList, size_t index);

int showNote(const ContactList* contactList, const size_t index);

int editNote(ContactList* contactList, const size_t index,
			 const char* first_name, const char* second_name,
			 const char* email);