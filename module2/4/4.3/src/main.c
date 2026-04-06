#include <stdio.h>
#include <stdlib.h>

#include "contactList.h"
#include "menu.h"

int main() {
	ContactList* root = NULL;
	size_t choice;

	system("clear");
	printMenu();

	while (scanf("%lu", &choice) == 1) {
		switch (choice) {
			case 0:
				freeContactList(&root);
				return 0;
			case 1:
				printf("------------------------------------------\n");
				showContactList(root);
				printf("------------------------------------------\n");
				break;
			case 2:
				addContactMenu(&root);
				break;
			case 3:
				editContactMenu(&root);
				break;
			case 4:
				rmContactMenu(&root);
				break;
			case 5:
				takeInfoMenu(root);
				break;
			case 6:
				if (root == NULL) {
					printf("Дерево пусто.\n");
				} else {
					printf("\nСтруктура дерева контактов:\n");
					printTree(root, "", 0);
				}
				break;
			case 7:
				system("clear");
				break;
		}
		printMenu();
	}
	freeContactList(&root);
	return 0;
}