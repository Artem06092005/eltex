#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "contactList.h"

void saveToFile(ContactList* contactList, const char* filePath) {
	if (!contactList || contactList->head == NULL) {
		return;
	}

	int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		perror("Error:open(file)");
		exit(EXIT_FAILURE);
	}
	Note* temp = contactList->head;
	while (temp) {
		write(fd, &temp->data, sizeof(temp->data));
		temp = temp->next;
	}
	close(fd);
}

void loadFile(ContactList* contactList, const char* filePath) {
	int fd = open(filePath, O_RDONLY);
	if (fd == -1) {
		return;
	}
	Data newData;
	while (read(fd, &newData, sizeof(Data)) == sizeof(Data)) {
		addContact(contactList, newData.firstName, newData.secondName,
				   newData.email);
	}
	close(fd);
}