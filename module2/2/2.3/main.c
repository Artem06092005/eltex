#include <math.h>
#include <stdio.h>

#include "calc.h"

Command commands[] = {{"Сложение", sum_two}, {"Вычитание", sub},
					  {"Умножение", mul},	 {"Деление", div},
					  {"Максимум", max2},	 {"fjafhjakja", div}};

const int num_commands = sizeof(commands) / sizeof(Command);

void printMenu() {
	printf("\n--- Динамический калькулятор ---\n");
	printf("0. Выход\n");
	for (int i = 0; i < num_commands; i++) {
		printf("%d. %s\n", i + 1, commands[i].name);
	}
	printf("--------------------------------\n");
	printf("Выберите действие: ");
}

int main() {
	int choice;
	double a, b, res;

	while (1) {
		printMenu();
		if (scanf("%d", &choice) != 1) break;

		if (choice == 0) {
			printf("Выход из программы.\n");
			break;
		}

		if (choice < 1 || choice > num_commands) {
			printf("Ошибка: Неверный пункт меню.\n");
			continue;
		}

		printf("Введите два числа через пробел: ");
		if (scanf("%lf %lf", &a, &b) != 2) {
			printf("Ошибка: Некорректный ввод.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		res = commands[choice - 1].func(a, b);

		if (res == INFINITY) {
			printf("Результат: Ошибка (деление на ноль или переполнение)\n");
		} else {
			printf("Результат (%s): %.2f\n", commands[choice - 1].name, res);
		}
	}
	return 0;
}