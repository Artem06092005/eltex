#pragma once

#include <stddef.h>

enum { max_length_link = 100 };

typedef struct Data {
	size_t quantityLink;
	char secondName[40];
	char firstName[40];
	char email[40];
	char** socialMediaLink;
} Data;

typedef struct ContactList {
	struct ContactList* left;
	struct ContactList* right;
	Data* data;
} ContactList;

void printTree(ContactList* root, char* prefix, int isLeft);
void editNote(ContactList** root, char* oldKey, char* newFirstName,
			  char* newSecondName, char* newEmail, size_t* newQty,
			  char** newLinks);
void showNote(ContactList* root, char* secondName);
void showContactList(ContactList* root);
int DeleteNote(char* key, ContactList** node);
void inputNote(char* firstName, char* secondName, char* email,
			   size_t quantityLink, char** socialMediaLink,
			   ContactList** btree);
void freeContactList(ContactList** root);