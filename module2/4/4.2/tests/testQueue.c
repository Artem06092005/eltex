#include <stdlib.h>

#include "../src/queue.h"
#include "../unity/unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_enqueue(void) {
	Queue q;
	initQueue(&q);
	TEST_ASSERT_NULL(q.head);

	enqueue(&q, 0, 1);
	enqueue(&q, 255, 3);
	enqueue(&q, 22, 2);

	Node* n = q.head;
	TEST_ASSERT_NOT_NULL(n);
	TEST_ASSERT_EQUAL_INT(255, n->priority);

	TEST_ASSERT_EQUAL_INT(22, n->next->priority);

	TEST_ASSERT_EQUAL_INT(0, n->next->next->priority);

	freeQueue(&q);
}

void test_DequeueFirst(void) {
	Queue q;
	initQueue(&q);

	enqueue(&q, 5, 55);
	enqueue(&q, 15, 155);

	int res = dequeueFirst(&q);
	TEST_ASSERT_EQUAL_INT(0, res);
	TEST_ASSERT_EQUAL_INT(5, q.head->priority);

	dequeueFirst(&q);
	TEST_ASSERT_NULL(q.head);

	TEST_ASSERT_EQUAL_INT(-1, dequeueFirst(&q));
}

void test_DequeueByPriority(void) {
	Queue q;
	initQueue(&q);

	enqueue(&q, 10, 1);
	enqueue(&q, 20, 2);
	enqueue(&q, 30, 3);

	int status = dequeueByPriority(&q, 20);
	TEST_ASSERT_EQUAL_INT(0, status);

	TEST_ASSERT_EQUAL_INT(30, q.head->priority);
	TEST_ASSERT_EQUAL_INT(10, q.head->next->priority);

	TEST_ASSERT_EQUAL_INT(-1, dequeueByPriority(&q, 99));

	freeQueue(&q);
}

void test_dequeueByMinPriority(void) {
	Queue q;
	initQueue(&q);

	enqueue(&q, 50, 500);
	enqueue(&q, 40, 400);
	enqueue(&q, 30, 300);

	int res = dequeueByMinPriority(&q, 35);
	TEST_ASSERT_EQUAL_INT(0, res);
	TEST_ASSERT_EQUAL_INT(40, q.head->priority);

	TEST_ASSERT_EQUAL_INT(-1, dequeueByMinPriority(&q, 100));

	freeQueue(&q);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_enqueue);
	RUN_TEST(test_DequeueFirst);
	RUN_TEST(test_DequeueByPriority);
	RUN_TEST(test_dequeueByMinPriority);
	return UNITY_END();
}