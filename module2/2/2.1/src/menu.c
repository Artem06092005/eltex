#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contactList.h"

void printMenu() {
	printf("--------Список контактов--------\n");
	printf("Напишите цифру для выбора действия\n");
	printf("0. Выйти из программы\n");
	printf("1. Вывод списка добаленных контактов\n");
	printf("2. Добавить контакт\n");
	printf("3. Редактирование контакта(введите номер контакта)\n");
	printf("4. Удаление контакта(введите id контакта)\n");
	printf("5. Вывод подробной информации о контакте\n");
	printf("6. Почистить консоль\n");
	printf("--------------------------------------------------\n");
}

int addContactMenu(ContactList* contactList) {
	printf("--------Добавление контакта--------\n");
	printf("В любой момент напишите -1,чтобы вернуться в главное меню\n");
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
	Note note = {0};
	char buffer[100];

	printf("Имя контакта (обязательно): ");
	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strcmp(buffer, "-1") == 0) return -1;
		if (strlen(buffer) > 0) {
			strncpy(note.firstName, buffer, sizeof(note.firstName) - 1);
			note.firstName[sizeof(note.firstName) - 1] = '\0';
			break;
		}
		printf("Имя не может быть пустым. Попробуйте ещё раз: ");
	}

	printf("Фамилия контакта (обязательно): ");
	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strcmp(buffer, "-1") == 0) return -1;
		if (strlen(buffer) > 0) {
			strncpy(note.secondName, buffer, sizeof(note.secondName) - 1);
			note.secondName[sizeof(note.secondName) - 1] = '\0';
			break;
		}
		printf("Фамилия не может быть пустой. Попробуйте ещё раз: ");
	}

	printf("Email контакта (Enter - пропустить): ");
	if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
	buffer[strcspn(buffer, "\n")] = '\0';
	if (strcmp(buffer, "-1") == 0) return -1;
	if (strlen(buffer) > 0) {
		strncpy(note.email, buffer, sizeof(note.email) - 1);
		note.email[sizeof(note.email) - 1] = '\0';
	} else {
		note.email[0] = '\0';
	}

	int val;
	while (1) {
		printf("Число ссылок на соцсети: ");
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strcmp(buffer, "-1") == 0) return -1;

		char* endptr;
		val = strtol(buffer, &endptr, 10);
		if (*endptr == '\0' && val >= 0) {
			break;
		}
		printf("Неверный ввод. Введите неотрицательное число\n");
	}
	note.quantityLink = (size_t)val;

	if (note.quantityLink > 0) {
		if (initSocialMediaLink(&note, note.quantityLink) == -1) return -1;

		for (size_t i = 0; i < note.quantityLink; i++) {
			printf("%lu ссылка: ", i);
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
			buffer[strcspn(buffer, "\n")] = '\0';
			if (strcmp(buffer, "-1") == 0) return -1;
			if (strlen(buffer) > 0) {
				strncpy(note.socialMediaLink[i], buffer, max_length_link - 1);
				note.socialMediaLink[i][max_length_link - 1] = '\0';
			} else {
				note.socialMediaLink[i][0] = '\0';
			}
		}
	}

	int res = addContactFromNote(contactList, &note);
	for (int i = 0; i < note.quantityLink; i++) {
		free(note.socialMediaLink[i]);
	}
	free(note.socialMediaLink);

	if (res == -1) return -2;
	return 0;
}

int rmContactMenu(ContactList* contactList) {
	printf("--------Удаление контакта--------\n");
	printf(
		"Введите id удалянмого элемента или -1, чтобы вернутся "
		"назад\n");

	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;

	char buffer[100];
	if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
	buffer[strcspn(buffer, "\n")] = '\0';

	if (strcmp(buffer, "-1") == 0) return -1;

	char* endptr;
	long id = strtol(buffer, &endptr, 10);
	if (*endptr != '\0' || id < 0) {
		printf("Неверный ввод\n");
		return -1;
	}
	if (rmNote(contactList, (size_t)id) == -1) {
		printf("Контакт с id %ld не найден\n", id);
	} else {
		printf("Контакт удалён\n");
	}
	return 0;
}

int takeInfoMenu(ContactList* contactList) {
	printf("--------Подробная информация о контакте--------\n");
	printf("Введите id контакта\n");

	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;

	char buffer[100];
	if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
	buffer[strcspn(buffer, "\n")] = '\0';

	if (strcmp(buffer, "-1") == 0) return -1;

	char* endptr;
	long id = strtol(buffer, &endptr, 10);
	if (*endptr != '\0' || id < 0) {
		printf("Неверный ввод\n");
		return -1;
	}
	if (showNote(contactList, (size_t)id) == -1) {
		printf("Контакт с id %ld не найден\n", id);
		return -1;
	}
	return 0;
}

int editContactMenu(ContactList* contactList) {
	printf("--------Редактирование контакта--------\n");
	printf("Введите id контакта (-1 для выхода):\n");

	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;

	char buffer[100];
	if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
	buffer[strcspn(buffer, "\n")] = '\0';

	if (strcmp(buffer, "-1") == 0) return -1;

	char* endptr;
	long id = strtol(buffer, &endptr, 10);
	if (*endptr != '\0' || id < 0) {
		printf("Неверный ввод\n");
		return -1;
	}

	if (showNote(contactList, (size_t)id) == -1) {
		printf("Контакт с id %ld не найден\n", id);
		return -1;
	}

	printf("\n--- Введите новые данные (Enter - оставить без изменений) ---\n");

	Note newData = {0};
	int updateLinks = 0;
	size_t newQuantity = 0;

	printf("Имя: ");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strlen(buffer) > 0) {
			strncpy(newData.firstName, buffer, sizeof(newData.firstName) - 1);
		}
	}

	printf("Фамилия: ");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strlen(buffer) > 0) {
			strncpy(newData.secondName, buffer, sizeof(newData.secondName) - 1);
		}
	}

	printf("Email: ");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strlen(buffer) > 0) {
			strncpy(newData.email, buffer, sizeof(newData.email) - 1);
		}
	}

	printf("Изменить ссылки? (y/n): ");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		if (buffer[0] == 'y' || buffer[0] == 'Y') {
			updateLinks = 1;
			int val;
			while (1) {
				printf("Число ссылок на соцсети: ");
				if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
				buffer[strcspn(buffer, "\n")] = '\0';
				if (strcmp(buffer, "-1") == 0) return -1;

				char* eptr;
				val = strtol(buffer, &eptr, 10);
				if (*eptr == '\0' && val >= 0) {
					break;
				}
				printf("Неверный ввод. Введите неотрицательное число\n");
			}
			newQuantity = (size_t)val;

			if (newQuantity > 0) {
				newData.socialMediaLink = malloc(sizeof(char*) * newQuantity);
				for (size_t i = 0; i < newQuantity; i++) {
					newData.socialMediaLink[i] = malloc(max_length_link);
					printf("%lu ссылка: ", i);
					if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
					buffer[strcspn(buffer, "\n")] = '\0';
					strncpy(newData.socialMediaLink[i], buffer,
							max_length_link - 1);
				}
			}
		}
	}

	if (editNote(contactList, (size_t)id,
				 newData.firstName[0] ? newData.firstName : NULL,
				 newData.secondName[0] ? newData.secondName : NULL,
				 newData.email[0] ? newData.email : NULL,
				 updateLinks ? &newQuantity : NULL,
				 updateLinks ? (const char**)newData.socialMediaLink : NULL) ==
		-1) {
		printf("Ошибка редактирования\n");
		return -1;
	}

	if (updateLinks && newQuantity > 0) {
		for (size_t i = 0; i < newQuantity; i++) {
			free(newData.socialMediaLink[i]);
		}
		free(newData.socialMediaLink);
	}

	printf("Контакт обновлён\n");
	return 0;
}