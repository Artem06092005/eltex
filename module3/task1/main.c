#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int isNum(char* s) {
	int i = 0;
	if (s[i] == '\0') return 0;
	if (s[i] == '-') i++;

	int floatFlag = 0;

	while (s[i] != '\0') {
		if (isdigit(s[i])) {
			i++;
		} else if (s[i] == '.') {
			if (floatFlag == 1) return 0;
			floatFlag = 1;
			i++;
		} else {
			return 0;
		}
	}
	if (floatFlag) return 2;
	return 1;
}

void printInt(char* s) {
	long res = strtol(s, NULL, 10);
	printf("%ld %ld\n", res, res * 2);
}
void printDouble(char* s) {
	double res = strtod(s, NULL);
	printf("%lf %lf\n", res, res * 2);
}

void change_print(char* s) {
	switch (isNum(s)) {
		case 0:
			printf("%s\n", s);
			break;
		case 1:
			printInt(s);
			break;
		case 2:
			printDouble(s);
			break;
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Неверное число параметров запуска\n");
		return 0;
	}
	pid_t pid;
	switch (pid = fork()) {
		case -1:
			perror("Error: fork()");
			exit(EXIT_FAILURE);
			break;
		case 0:
			for (size_t i = 1; i <= (argc - 1) / 2; i++) {
				change_print(argv[i]);
			}
			exit(EXIT_SUCCESS);
			break;
		default:
			for (size_t i = (argc - 1) / 2 + 1; i < argc; i++) {
				change_print(argv[i]);
			}
			break;
	}
}