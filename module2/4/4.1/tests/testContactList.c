#include <stdio.h>
#include <string.h>

#include "../src/contactList.h"
#include "../unity/unity.h"

void setUp() {}
void tearDown() {}

void test_input_rm_Note(void) {
	ContactList list;
	initContactList(&list);
	char* names[] = {"user1", "user2", "user3", "user4", "user5"};
	char* fams[] = {"fam1", "fam2", "fam3", "fam4", "fam5"};

	for (size_t i = 0; i < sizeof(fams) / sizeof(fams[0]); i++) {
		addContact(&list, names[i], fams[i], NULL, 0, NULL);
	}
	TEST_ASSERT_EQUAL(5, list.size);
	TEST_ASSERT_EQUAL(list.size, sizeof(fams) / sizeof(fams[0]));
	Note* curr = list.head;

	size_t i = 0;
	while (curr) {
		TEST_ASSERT_EQUAL_STRING(names[i], curr->firstName);
		TEST_ASSERT_EQUAL_STRING(fams[i], curr->secondName);
		curr = curr->next;
		i++;
	}

	rmNote(&list, 4);
	TEST_ASSERT_EQUAL_STRING(fams[3], list.tail->secondName);
	TEST_ASSERT_EQUAL(4, list.size);

	rmNote(&list, 0);
	TEST_ASSERT_EQUAL_STRING(fams[1], list.head->secondName);
	TEST_ASSERT_EQUAL(3, list.size);

	rmNote(&list, 1);
	TEST_ASSERT_EQUAL_STRING(fams[1], list.head->secondName);
	TEST_ASSERT_EQUAL_STRING(fams[3], list.tail->secondName);

	TEST_ASSERT_EQUAL(2, list.size);

	freeContactList(&list);

	TEST_ASSERT_NULL(list.head);
	TEST_ASSERT_NULL(list.tail);
	TEST_ASSERT_EQUAL(0, list.size);
}

void test_EditNote(void) {
	ContactList list;
	initContactList(&list);
	char* names[] = {"user1", "user2", "user3", "user4", "user5"};
	char* fams[] = {"fam1", "fam2", "fam3", "fam4", "fam5"};

	for (size_t i = 0; i < sizeof(fams) / sizeof(fams[0]); i++) {
		addContact(&list, names[i], fams[i], NULL, 0, NULL);
	}
	editNote(&list, 2, NULL, "000", NULL, 0, NULL);
	editNote(&list, 4, NULL, "zzzz99", NULL, 0, NULL);

	TEST_ASSERT_EQUAL_STRING("000", list.head->secondName);
	TEST_ASSERT_EQUAL_STRING("fam1", list.head->next->secondName);
	TEST_ASSERT_EQUAL_STRING("fam2", list.head->next->next->secondName);
	TEST_ASSERT_EQUAL_STRING("fam4", list.head->next->next->next->secondName);
	TEST_ASSERT_EQUAL_STRING("zzzz99", list.tail->secondName);

	freeContactList(&list);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_input_rm_Note);
	RUN_TEST(test_EditNote);
	return UNITY_END();
}