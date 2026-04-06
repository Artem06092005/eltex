#pragma once

#include "contactList.h"

void printMenu();
int addContactMenu(ContactList** root);
int rmContactMenu(ContactList** root);
int takeInfoMenu(ContactList* root);
int editContactMenu(ContactList** root);