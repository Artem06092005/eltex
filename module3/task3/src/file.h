#pragma once

#include "contactList.h"

void loadFile(ContactList* contactList, const char* filePath);
void saveToFile(ContactList* contactList, const char* filePath);