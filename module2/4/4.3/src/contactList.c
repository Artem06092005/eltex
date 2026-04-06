#include "contactList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void addContact(Data* data, ContactList** btree) {
	if (*btree == NULL) {
		*btree = malloc(sizeof(ContactList));
		(*btree)->left = (*btree)->right = NULL;
		(*btree)->data = data;
		data = NULL;
		return;
	}
	if (strcmp(data->secondName, (*btree)->data->secondName) == 0) {
		if (strcmp(data->firstName, (*btree)->data->firstName) < 0)
			addContact(data, &(*btree)->left);
		else
			addContact(data, &(*btree)->right);
	} else if (strcmp(data->secondName, (*btree)->data->secondName) < 0)
		addContact(data, &(*btree)->left);
	else {
		addContact(data, &(*btree)->right);
	}
}

void inputNote(char* firstName, char* secondName, char* email,
			   size_t quantityLink, char** socialMediaLink,
			   ContactList** btree) {
	Data* data = malloc(sizeof(Data));
	if (firstName && strlen(firstName) > 0) {
		strncpy(data->firstName, firstName, sizeof(data->firstName) - 1);
		data->firstName[sizeof(data->firstName) - 1] = '\0';
	}
	if (secondName && strlen(secondName)) {
		strncpy(data->secondName, secondName, sizeof(data->secondName) - 1);
		data->secondName[sizeof(data->secondName) - 1] = '\0';
	}
	if (email && strlen(email) > 0) {
		strncpy(data->email, email, sizeof(data->email) - 1);
		data->email[sizeof(data->email) - 1] = '\0';
	}
	data->quantityLink = quantityLink;
	data->socialMediaLink = malloc(sizeof(char*) * quantityLink);
	for (size_t i = 0; i < quantityLink; i++) {
		data->socialMediaLink[i] = malloc(max_length_link * sizeof(char));
		if (socialMediaLink[i] && strlen(socialMediaLink[i]) > 0) {
			strncpy(data->socialMediaLink[i], socialMediaLink[i],
					max_length_link - 1);
			data->socialMediaLink[i][max_length_link - 1] = '\0';
		}
	}
	addContact(data, btree);
}

static void freeLinks(size_t size, char** socialMediaLink) {
	for (size_t i = 0; i < size; i++) {
		free(socialMediaLink[i]);
	}
	free(socialMediaLink);
}

int DeleteNote(char* key, ContactList** node) {
	ContactList *t, *up;
	if (*node == NULL) return 0;

	if (strcmp((*node)->data->secondName, key) == 0) {
		if (((*node)->left == NULL) && ((*node)->right == NULL)) {
			freeLinks((*node)->data->quantityLink,
					  (*node)->data->socialMediaLink);
			free((*node)->data);
			free(*node);
			*node = NULL;
			return 1;
		}

		if ((*node)->left == NULL) {
			t = *node;
			*node = (*node)->right;
			freeLinks(t->data->quantityLink, t->data->socialMediaLink);
			free(t->data);
			free(t);
			return 1;
		}
		if ((*node)->right == NULL) {
			t = *node;
			*node = (*node)->left;
			freeLinks(t->data->quantityLink, t->data->socialMediaLink);
			free(t->data);
			free(t);
			return 1;
		}

		up = *node;
		t = (*node)->left;

		while (t->right != NULL) {
			up = t;
			t = t->right;
		}

		freeLinks((*node)->data->quantityLink, (*node)->data->socialMediaLink);
		free((*node)->data);

		(*node)->data = t->data;

		if (up != (*node)) {
			up->right = t->left;
		} else {
			(*node)->left = t->left;
		}

		free(t);
		return 1;
	}

	if (strcmp((*node)->data->secondName, key) < 0)
		return DeleteNote(key, &(*node)->right);
	return DeleteNote(key, &(*node)->left);
}

void showContactList(ContactList* root) {
	if (root == NULL) return;

	showContactList(root->left);
	printf("Name: %-15s | Second Name: %-15s\n", root->data->firstName,
		   root->data->secondName);
	showContactList(root->right);
}

void showNote(ContactList* root, char* secondName) {
	if (root == NULL) {
		printf("Контакт с фамилией %s не найден.\n", secondName);
		return;
	}

	int cmp = strcmp(secondName, root->data->secondName);
	if (cmp == 0) {
		printf("\n--- Информация о контакте ---\n");
		printf("Имя: %s\nФамилия: %s\nEmail: %s\n", root->data->firstName,
			   root->data->secondName, root->data->email);
		printf("Ссылки (%zu):\n", root->data->quantityLink);
		for (size_t i = 0; i < root->data->quantityLink; i++) {
			printf("  [%zu]: %s\n", i + 1, root->data->socialMediaLink[i]);
		}
	} else if (cmp < 0) {
		showNote(root->left, secondName);
	} else {
		showNote(root->right, secondName);
	}
}

static Data* copyData(Data* src) {
	Data* dest = malloc(sizeof(Data));
	memcpy(dest, src, sizeof(Data));
	dest->socialMediaLink = malloc(sizeof(char*) * src->quantityLink);
	for (size_t i = 0; i < src->quantityLink; i++) {
		dest->socialMediaLink[i] = malloc(max_length_link);
		strcpy(dest->socialMediaLink[i], src->socialMediaLink[i]);
	}
	return dest;
}

void editNote(ContactList** root, char* oldKey, char* newFirstName,
			  char* newSecondName, char* newEmail, size_t* newQty,
			  char** newLinks) {
	ContactList* node = *root;
	while (node && strcmp(node->data->secondName, oldKey) != 0) {
		if (strcmp(oldKey, node->data->secondName) < 0)
			node = node->left;
		else
			node = node->right;
	}

	if (!node) {
		printf("Ошибка: контакт с фамилией %s не найден.\n", oldKey);
		return;
	}

	Data* newData = copyData(node->data);

	if (newFirstName && strlen(newFirstName) > 0)
		strcpy(newData->firstName, newFirstName);
	if (newSecondName && strlen(newSecondName) > 0)
		strcpy(newData->secondName, newSecondName);
	if (newEmail && strlen(newEmail) > 0) strcpy(newData->email, newEmail);

	if (newQty && newLinks) {
		freeLinks(newData->quantityLink, newData->socialMediaLink);

		newData->quantityLink = *newQty;
		newData->socialMediaLink =
			malloc(sizeof(char*) * newData->quantityLink);
		for (size_t i = 0; i < newData->quantityLink; i++) {
			newData->socialMediaLink[i] = malloc(max_length_link);
			if (newLinks[i]) {
				strncpy(newData->socialMediaLink[i], newLinks[i],
						max_length_link - 1);
				newData->socialMediaLink[i][max_length_link - 1] = '\0';
			}
		}
	}
	DeleteNote(oldKey, root);
	addContact(newData, root);
	printf("Контакт успешно обновлен.\n");
}

void printTree(ContactList* root, char* prefix, int isLeft) {
	if (root == NULL) return;

	printf("%s", prefix);
	printf("%s", (isLeft ? "├── " : "└── "));
	printf("[%s]\n", root->data->secondName);

	char newPrefix[256];
	sprintf(newPrefix, "%s%s", prefix, (isLeft ? "│   " : "    "));

	if (root->left || root->right) {
		if (root->left) {
			printTree(root->left, newPrefix, 1);
		} else {
			printf("%s├── (пусто)\n", newPrefix);
		}

		if (root->right) {
			printTree(root->right, newPrefix, 0);
		}
	}
}

void freeContactList(ContactList** root) {
	if (*root == NULL) return;

	freeContactList(&((*root)->left));
	freeContactList(&((*root)->right));

	if ((*root)->data) {
		freeLinks((*root)->data->quantityLink, (*root)->data->socialMediaLink);
		free((*root)->data);
	}

	free(*root);
	*root = NULL;
}