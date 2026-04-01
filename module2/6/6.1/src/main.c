#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contactList.h"
#include "menu.h"

int main() {
	ContactList contactList;
	initContactList(&contactList);
	size_t choice;
	system("clear");
	printMenu();
	while (scanf("%lu", &choice) == 1) {
		switch (choice) {
			case 0:
				freeContactList(&contactList);
				return 0;
			case 1:
				printf("------------------------------------------\n");
				showContactList(&contactList);
				printf("------------------------------------------\n");
				break;
			case 2:
				int res = addContactMenu(&contactList);
				if (res == -2) {
					printf("MemError,память не правильно выделилась");
					return -1;
				}
				break;
			case 3:
				editContactMenu(&contactList);
				break;

			case 4:
				rmContactMenu(&contactList);
				break;

			case 5:
				takeInfoMenu(&contactList);
				break;
			case 6:
				system("clear");
				break;
		}
		printMenu();
	}
	freeContactList(&contactList);
	return 0;
}