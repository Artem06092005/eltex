#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contactList.h"

static void clearInput() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void printMenu() {
	printf("\n-------- Список контактов (BST) --------\n");
	printf("0. Выйти из программы\n");
	printf("1. Вывод списка (алфавитный)\n");
	printf("2. Добавить контакт\n");
	printf("3. Редактировать контакт (по фамилии)\n");
	printf("4. Удалить контакт (по фамилии)\n");
	printf("5. Подробная информация (по фамилии)\n");
	printf("6. Показать структуру дерева\n");
	printf("7. Очистить консоль\n");
	printf("----------------------------------------\n");
	printf("Выберите действие: ");
}

int addContactMenu(ContactList** root) {
	char fName[40], sName[40], mail[40], buffer[100];
	int qty = 0;

	printf("\n--- Добавление контакта ---\n");
	clearInput();

	while (1) {
		printf("Имя (обязательно): ");
		if (!fgets(fName, sizeof(fName), stdin)) return -1;
		fName[strcspn(fName, "\n")] = '\0';
		if (strlen(fName) > 0) break;
		printf("Ошибка: Имя не может быть пустым.\n");
	}

	while (1) {
		printf("Фамилия (обязательно): ");
		if (!fgets(sName, sizeof(sName), stdin)) return -1;
		sName[strcspn(sName, "\n")] = '\0';
		if (strlen(sName) > 0) break;
		printf("Ошибка: Фамилия не может быть пустой.\n");
	}

	printf("Email: ");
	fgets(mail, sizeof(mail), stdin);
	mail[strcspn(mail, "\n")] = '\0';

	printf("Количество ссылок: ");
	if (fgets(buffer, sizeof(buffer), stdin)) {
		qty = atoi(buffer);
		if (qty < 0) qty = 0;
	}

	char** links = NULL;
	if (qty > 0) {
		links = malloc(sizeof(char*) * qty);
		for (int i = 0; i < qty; i++) {
			links[i] = malloc(max_length_link);
			printf("Ссылка %d: ", i + 1);
			fgets(links[i], max_length_link, stdin);
			links[i][strcspn(links[i], "\n")] = '\0';
		}
	}

	inputNote(fName, sName, mail, (size_t)qty, links, root);

	if (links) {
		for (int i = 0; i < qty; i++) free(links[i]);
		free(links);
	}

	printf("Контакт успешно добавлен!\n");
	return 0;
}

int rmContactMenu(ContactList** root) {
	char key[40];
	printf("Введите фамилию для удаления: ");
	clearInput();
	if (scanf("%39s", key) != 1) return -1;

	if (DeleteNote(key, root)) {
		printf("Контакт удален.\n");
	} else {
		printf("Ошибка: контакт с фамилией %s не найден.\n", key);
	}
	return 0;
}

int takeInfoMenu(ContactList* root) {
	char key[40];
	printf("Введите фамилию для поиска: ");
	clearInput();
	if (scanf("%39s", key) != 1) return -1;
	showNote(root, key);
	return 0;
}

int editContactMenu(ContactList** root) {
	char oldKey[40], fName[40], sName[40], mail[40], buffer[100];

	printf("Введите фамилию контакта для редактирования: ");
	clearInput();
	if (scanf("%39s", oldKey) != 1) return -1;
	clearInput();

	printf("Новое имя (Enter — оставить прежнее): ");
	fgets(fName, sizeof(fName), stdin);
	fName[strcspn(fName, "\n")] = '\0';

	printf("Новая фамилия (Enter — оставить прежнее): ");
	fgets(sName, sizeof(sName), stdin);
	sName[strcspn(sName, "\n")] = '\0';

	printf("Новый Email (Enter — оставить прежнее): ");
	fgets(mail, sizeof(mail), stdin);
	mail[strcspn(mail, "\n")] = '\0';

	printf("Обновить ссылки? (y/n): ");
	fgets(buffer, sizeof(buffer), stdin);

	size_t newQty = 0;
	char** newLinks = NULL;
	size_t* pQty = NULL;

	if (buffer[0] == 'y' || buffer[0] == 'Y') {
		printf("Новое количество ссылок: ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
			newQty = (size_t)atoi(buffer);
			pQty = &newQty;

			if (newQty > 0) {
				newLinks = malloc(sizeof(char*) * newQty);
				for (size_t i = 0; i < newQty; i++) {
					newLinks[i] = malloc(max_length_link);
					printf("Новая ссылка %zu: ", i + 1);
					fgets(newLinks[i], max_length_link, stdin);
					newLinks[i][strcspn(newLinks[i], "\n")] = '\0';
				}
			}
		}
	}

	editNote(root, oldKey, strlen(fName) ? fName : NULL,
			 strlen(sName) ? sName : NULL, strlen(mail) ? mail : NULL, pQty,
			 newLinks);

	if (newLinks) {
		for (size_t i = 0; i < newQty; i++) free(newLinks[i]);
		free(newLinks);
	}
	return 0;
}