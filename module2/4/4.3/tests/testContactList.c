#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/contactList.h"
#include "../unity/unity.h"

int countNodes(ContactList* root) {
	if (root == NULL) return 0;
	return 1 + countNodes(root->left) + countNodes(root->right);
}

void setUp() {}
void tearDown() {}

void test_Add(void) {
	ContactList* root = NULL;

	inputNote("Ivan", "M-Sname", "mail1@test.ru", 0, NULL, &root);
	inputNote("Petr", "A-Sname", "mail2@test.ru", 0, NULL, &root);
	inputNote("Sidor", "Z-Sname", "mail3@test.ru", 0, NULL, &root);

	TEST_ASSERT_NOT_NULL(root);
	TEST_ASSERT_EQUAL_INT(3, countNodes(root));

	TEST_ASSERT_EQUAL_STRING("M-Sname", root->data->secondName);
	TEST_ASSERT_NOT_NULL(root->left);
	TEST_ASSERT_EQUAL_STRING("A-Sname", root->left->data->secondName);
	TEST_ASSERT_NOT_NULL(root->right);
	TEST_ASSERT_EQUAL_STRING("Z-Sname", root->right->data->secondName);

	freeContactList(&root);
	TEST_ASSERT_NULL(root);
}

void test_Delete(void) {
	ContactList* root = NULL;

	inputNote("User1", "B-Fam", "mail", 0, NULL, &root);
	inputNote("User2", "A-Fam", "mail", 0, NULL, &root);
	inputNote("User3", "C-Fam", "mail", 0, NULL, &root);

	int res = DeleteNote("A-Fam", &root);
	TEST_ASSERT_EQUAL_INT(1, res);
	TEST_ASSERT_EQUAL_INT(2, countNodes(root));
	TEST_ASSERT_NULL(root->left);
	res = DeleteNote("NonExistent", &root);
	TEST_ASSERT_EQUAL_INT(0, res);
	TEST_ASSERT_EQUAL_INT(2, countNodes(root));

	freeContactList(&root);
}

void test_Edit(void) {
	ContactList* root = NULL;

	inputNote("User1", "Ivanov", "mail", 0, NULL, &root);
	inputNote("User2", "Abramov", "mail", 0, NULL, &root);

	editNote(&root, "Abramov", NULL, "Yakovlev", NULL, NULL, NULL);

	TEST_ASSERT_EQUAL_INT(2, countNodes(root));
	TEST_ASSERT_EQUAL_STRING("Ivanov", root->data->secondName);
	TEST_ASSERT_NULL(root->left);
	TEST_ASSERT_NOT_NULL(root->right);
	TEST_ASSERT_EQUAL_STRING("Yakovlev", root->right->data->secondName);
	freeContactList(&root);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_Add);
	RUN_TEST(test_Delete);
	RUN_TEST(test_Edit);
	return UNITY_END();
}