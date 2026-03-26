#pragma once

#include <stddef.h>

enum { max_length_link = 100 };

typedef struct Note {
	size_t id;
	size_t quantityLink;
	char secondName[40];
	char firstName[40];
	char email[40];
	char **socialMediaLink;
} Note;

typedef struct ContactList {
	size_t quantityNotes;
	size_t size;
	Note *notes;
} ContactList;

int initContactList(ContactList *contactList, size_t size);

void freeContactList(ContactList *contactList);

void showContactList(ContactList *contactList);

int addContact(ContactList *contactList, const char *first_name,
			   const char *second_name, const char *email,
			   const size_t quantity_link, const char **social_media_link);

int addContactFromNote(ContactList *contactList, Note *note);

int initSocialMediaLink(Note *note, size_t l);

int findIndexById(const ContactList *contactList, const size_t id);

int rmNote(ContactList *contactList, size_t id);

int showNote(const ContactList *contactList, const size_t id);

int editNote(ContactList *contactList, const size_t id, const char *first_name,
			 const char *second_name, const char *email,
			 const size_t *quantity_link, const char **socialMediaLink);