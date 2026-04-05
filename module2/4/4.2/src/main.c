#include <queue.h>
#include <stdio.h>
#include <stdlib.h>

void printMenu() {
	printf("\n========== МЕНЮ ==========\n");
	printf("0. Выход\n");
	printf("1. Добавить элемент\n");
	printf("2. Извлечь первый элемент\n");
	printf("3. Извлечь элемент с указанным приоритетом\n");
	printf("4. Извлечь элемент с приоритетом не ниже заданного\n");
	printf("5. Показать очередь\n");
	printf("6. Очистить очередь\n");
	printf("==========================\n");
	printf("Ваш выбор: ");
}

void printQueue(const Queue* queue) {
	if (queue->head == NULL) {
		printf("Очередь пуста\n");
		return;
	}

	printf("Очередь:\n");
	Node* current = queue->head;
	int pos = 1;
	while (current != NULL) {
		printf("  %d. [приоритет=%d, значение=%d]\n", pos++, current->priority,
			   current->val);
		current = current->next;
	}
}

void addElement(Queue* queue) {
	uint8_t priority;
	int val;

	printf("Введите приоритет (0-255): ");
	if (scanf("%hhu", &priority) != 1) {
		printf("Ошибка ввода!\n");
		while (getchar() != '\n')
			;
		return;
	}

	printf("Введите значение: ");
	if (scanf("%d", &val) != 1) {
		printf("Ошибка ввода!\n");
		while (getchar() != '\n')
			;
		return;
	}

	if (enqueue(queue, priority, val) == 0) {
		printf("Элемент успешно добавлен!\n");
	} else {
		printf("Ошибка при добавлении элемента!\n");
	}
}

void dequeueFirstElement(Queue* queue) {
	if (dequeueFirst(queue) == 0) {
		printf("Первый элемент успешно удален!\n");
	} else {
		printf("Очередь пуста! Невозможно удалить элемент.\n");
	}
}

void dequeueByPriorityElement(Queue* queue) {
	uint8_t priority;

	printf("Введите приоритет для удаления (0-255): ");
	if (scanf("%hhu", &priority) != 1) {
		printf("Ошибка ввода!\n");
		while (getchar() != '\n')
			;
		return;
	}

	if (dequeueByPriority(queue, priority) == 0) {
		printf("Элемент с приоритетом %d успешно удален!\n", priority);
	} else {
		printf("Элемент с приоритетом %d не найден!\n", priority);
	}
}

void dequeueByMinPriorityElement(Queue* queue) {
	uint8_t priority;

	printf("Введите минимальный приоритет (0-255): ");
	if (scanf("%hhu", &priority) != 1) {
		printf("Ошибка ввода!\n");
		while (getchar() != '\n')
			;
		return;
	}

	if (dequeueByMinPriority(queue, priority) == 0) {
		printf("Элемент с приоритетом не ниже %d успешно удален!\n", priority);
	} else {
		printf("Элемент с приоритетом не ниже %d не найден!\n", priority);
	}
}

void clearQueue(Queue* queue) {
	freeQueue(queue);
	initQueue(queue);
	printf("Очередь очищена!\n");
}

int main() {
	Queue queue;
	initQueue(&queue);
	int choice;

	do {
		printMenu();
		if (scanf("%d", &choice) != 1) {
			printf("Ошибка ввода! Пожалуйста, введите число.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		switch (choice) {
			case 1:
				addElement(&queue);
				break;
			case 2:
				dequeueFirstElement(&queue);
				break;
			case 3:
				dequeueByPriorityElement(&queue);
				break;
			case 4:
				dequeueByMinPriorityElement(&queue);
				break;
			case 5:
				printQueue(&queue);
				break;
			case 6:
				clearQueue(&queue);
				break;
			case 0:
				printf("Выход из программы...\n");
				break;
			default:
				printf(
					"Неверный выбор! Пожалуйста, выберите пункт от 0 до 6.\n");
		}
	} while (choice != 0);

	freeQueue(&queue);

	return 0;
}