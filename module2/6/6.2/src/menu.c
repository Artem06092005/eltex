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
	printf("\n-------- Список контактов --------\n");
	printf("0. Выйти из программы\n");
	printf("1. Вывод списка контактов\n");
	printf("2. Добавить контакт\n");
	printf("3. Редактировать контакт (по индексу)\n");
	printf("4. Удаление контакта (по индексу)\n");
	printf("5. Подробная информация о контакте\n");
	printf("6. Очистить консоль\n");
	printf("----------------------------------\n");
	printf("Выберите действие: ");
}

int addContactMenu(ContactList* contactList) {
	char firstName[40], secondName[40], email[40];
	char buffer[100];
	int qty = 0;

	printf("\n--- Добавление контакта ---\n");
	printf("(Введите -1 в любом поле для отмены)\n");

	clearInput();

	while (1) {
		printf("Имя (обязательно): ");
		if (!fgets(firstName, sizeof(firstName), stdin)) return -1;
		firstName[strcspn(firstName, "\n")] = '\0';

		if (strcmp(firstName, "-1") == 0) return -1;
		if (strlen(firstName) > 0) break;
		printf("Имя не может быть пустым.\n");
	}

	while (1) {
		printf("Фамилия (обязательно): ");
		if (!fgets(secondName, sizeof(secondName), stdin)) return -1;
		secondName[strcspn(secondName, "\n")] = '\0';

		if (strcmp(secondName, "-1") == 0) return -1;
		if (strlen(secondName) > 0) break;
		printf("Фамилия не может быть пустым.\n");
	}

	printf("Email: ");
	if (!fgets(email, sizeof(email), stdin)) return -1;
	email[strcspn(email, "\n")] = '\0';
	if (strcmp(email, "-1") == 0) return -1;

	printf("Количество ссылок: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
	qty = atoi(buffer);
	if (qty < 0) qty = 0;

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

	int res = addContact(contactList, firstName, secondName, email, qty,
						 (const char**)links);

	if (links) {
		for (int i = 0; i < qty; i++) free(links[i]);
		free(links);
	}

	if (res == 0) printf("Контакт успешно добавлен!\n");
	return res;
}

int rmContactMenu(ContactList* contactList) {
	size_t index;
	printf("Введите индекс для удаления: ");
	if (scanf("%lu", &index) != 1) {
		clearInput();
		return -1;
	}

	if (rmNote(contactList, index) == 0) {
		printf("Контакт удален.\n");
	} else {
		printf("Ошибка: контакт с таким индексом не найден.\n");
	}
	return 0;
}

int takeInfoMenu(ContactList* contactList) {
	size_t index;
	printf("Введите индекс контакта: ");
	if (scanf("%lu", &index) != 1) {
		clearInput();
		return -1;
	}

	if (showNote(contactList, index) != 0) {
		printf("Контакт не найден.\n");
	}
	return 0;
}

int editContactMenu(ContactList* contactList) {
	size_t index;
	printf("Введите индекс для редактирования: ");
	if (scanf("%lu", &index) != 1) {
		clearInput();
		return -1;
	}

	if (index >= contactList->size) {
		printf("Индекс вне диапазона.\n");
		return -1;
	}

	clearInput();
	char fName[40], sName[40], mail[40], buffer[100];

	printf("Новое имя (Enter чтобы оставить): ");
	fgets(fName, sizeof(fName), stdin);
	fName[strcspn(fName, "\n")] = '\0';

	printf("Новая фамилия (Enter чтобы оставить): ");
	fgets(sName, sizeof(sName), stdin);
	sName[strcspn(sName, "\n")] = '\0';

	printf("Новый Email (Enter чтобы оставить): ");
	fgets(mail, sizeof(mail), stdin);
	mail[strcspn(mail, "\n")] = '\0';

	printf("Обновить ссылки? (y/n): ");
	fgets(buffer, sizeof(buffer), stdin);

	size_t newQty = 0;
	char** newLinks = NULL;
	size_t* pQty = NULL;

	if (buffer[0] == 'y' || buffer[0] == 'Y') {
		printf("Новое количество ссылок: ");
		fgets(buffer, sizeof(buffer), stdin);
		newQty = (size_t)atoi(buffer);
		pQty = &newQty;

		if (newQty > 0) {
			newLinks = malloc(sizeof(char*) * newQty);
			for (size_t i = 0; i < newQty; i++) {
				newLinks[i] = malloc(max_length_link);
				printf("Ссылка %lu: ", i + 1);
				fgets(newLinks[i], max_length_link, stdin);
				newLinks[i][strcspn(newLinks[i], "\n")] = '\0';
			}
		}
	}
	int res = editNote(contactList, index, strlen(fName) ? fName : NULL,
					   strlen(sName) ? sName : NULL, strlen(mail) ? mail : NULL,
					   pQty, (const char**)newLinks);

	if (newLinks) {
		for (size_t i = 0; i < newQty; i++) free(newLinks[i]);
		free(newLinks);
	}

	if (res == 0) printf("Контакт обновлен.\n");
	return res;
}