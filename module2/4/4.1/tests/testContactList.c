#include <stdio.h>
#include <string.h>

#include "../src/contactList.h"
#include "../unity/unity.h"

void setUp() {}
void tearDown() {}

void test_initContactList(void) {
	ContactList list = {0};
	int size = 10;
	int res = initContactList(&list, size);
	TEST_ASSERT_EQUAL(res, 0);
	TEST_ASSERT_EQUAL(0, list.quantityNotes);
	TEST_ASSERT_EQUAL(size, list.size);
	TEST_ASSERT_NOT_NULL(list.notes);
	res = initContactList(&list, size);
	TEST_ASSERT_EQUAL(res, -2);
	freeContactList(&list);
	resetContactCounter();
}

void test_addContactVariadic(void) {
	ContactList list = {0};
	initContactList(&list, 5);

	int res = addContactVariadic(&list, "Ivan", "Ivanov", "mail.ru", 3, "ww1",
								 "ww2", "ww3");

	TEST_ASSERT_EQUAL(0, res);
	TEST_ASSERT_EQUAL(1, list.quantityNotes);
	TEST_ASSERT_EQUAL(3, list.notes[0].quantityLink);

	TEST_ASSERT_EQUAL_STRING("ww1", list.notes[0].socialMediaLink[0]);
	TEST_ASSERT_EQUAL_STRING("ww2", list.notes[0].socialMediaLink[1]);
	TEST_ASSERT_EQUAL_STRING("ww3", list.notes[0].socialMediaLink[2]);

	res = addContactVariadic(&list, "Oleg", "Petrov", NULL, 0);
	TEST_ASSERT_EQUAL(0, res);
	TEST_ASSERT_EQUAL(2, list.quantityNotes);
	TEST_ASSERT_NULL(list.notes[1].socialMediaLink);

	freeContactList(&list);
	resetContactCounter();
}

void test_findIndexById(void) {
	ContactList list = {0};
	initContactList(&list, 10);

	addContactVariadic(&list, "Ivan", "Ivanov", NULL, 0);
	addContactVariadic(&list, "Petr", "Petrov", NULL, 0);

	TEST_ASSERT_EQUAL_INT(0, findIndexById(&list, 1));
	TEST_ASSERT_EQUAL_INT(1, findIndexById(&list, 2));
	TEST_ASSERT_EQUAL_INT(-1, findIndexById(&list, 999));

	freeContactList(&list);
	resetContactCounter();
}

void test_rmNote(void) {
	ContactList list = {0};
	initContactList(&list, 10);

	addContactVariadic(&list, "hhw", "agagag", NULL, 0);
	addContactVariadic(&list, "tty", "gaganghmghj", NULL, 0);
	addContactVariadic(&list, "foo", "dhdhnve", NULL, 0);

	TEST_ASSERT_EQUAL_INT(3, list.quantityNotes);

	int result = rmNote(&list, 2);
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(2, list.quantityNotes);

	TEST_ASSERT_EQUAL_STRING("hhw", list.notes[0].firstName);
	TEST_ASSERT_EQUAL_STRING("foo", list.notes[1].firstName);

	freeContactList(&list);
	resetContactCounter();
}

void test_editNote(void) {
	ContactList list = {0};
	initContactList(&list, 5);

	addContactVariadic(&list, "User1", "FAm1", "user1@mail.com", 2, "link1",
					   "link2");

	int res = editNote(&list, 1, "Petr", "Petrov", NULL, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, res);
	TEST_ASSERT_EQUAL_STRING("Petr", list.notes[0].firstName);
	TEST_ASSERT_EQUAL_STRING("Petrov", list.notes[0].secondName);

	const char* newLinks[] = {"https://t.me/petr"};
	size_t newQuantity = 1;
	res = editNote(&list, 1, NULL, NULL, NULL, &newQuantity, newLinks);

	TEST_ASSERT_EQUAL_INT(0, res);
	TEST_ASSERT_EQUAL_INT(1, list.notes[0].quantityLink);
	TEST_ASSERT_EQUAL_STRING("https://t.me/petr",
							 list.notes[0].socialMediaLink[0]);

	freeContactList(&list);
	resetContactCounter();
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_initContactList);
	RUN_TEST(test_findIndexById);
	RUN_TEST(test_rmNote);
	RUN_TEST(test_editNote);
	RUN_TEST(test_addContactVariadic);
	return UNITY_END();
}