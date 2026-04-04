#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (*calc_func)(double, double);

typedef struct {
	const char* name;
	calc_func func;
} Command;

Command commands[] = {{"Сложение", NULL},
					  {"Вычитание", NULL},
					  {"Умножение", NULL},
					  {"Деление", NULL}};

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

void* loadFunc(char* libName, char* funcName, size_t index) {
	char* error;

	void* handle = dlopen(libName, RTLD_NOW);
	if (!handle) {
		fputs(dlerror(), stderr);
		exit(1);
	}
	commands[index].func = dlsym(handle, funcName);
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	return handle;
}

int main() {
	int choice;
	double a, b, res;
	void* handle = loadFunc("sum_two.so", "sum_two", 0);
	void* handle2 = loadFunc("sub.so", "sub", 1);
	void* handle3 = loadFunc("mul.so", "mul", 2);
	void* handle4 = loadFunc("div.so", "div", 3);

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
	dlclose(handle);
	dlclose(handle2);
	dlclose(handle3);
	dlclose(handle4);
	return 0;
}