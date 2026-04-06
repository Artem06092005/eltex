#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum { countArg = 50, bufferSize = 100 };

int main() {
	pid_t pid;
	int rv;
	char buffer[bufferSize];
	char *args[countArg];

	size_t i = 0;
	char *token, *comm;
	while (1) {
		printf("----------------------------------------\n");
		printf("Введите программу и аргументы запуска\n");
		printf("----------------------------------------\n");
		fgets(buffer, sizeof(buffer), stdin);

		buffer[strcspn(buffer, "\n")] = '\0';

		if (strcmp(buffer, "exit") == 0) break;

		i = 0;
		token = strtok(buffer, " ");
		while (token != NULL && i < countArg) {
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;

		switch (pid = fork()) {
			case -1:
				perror("Error: fork()\n");
				exit(EXIT_FAILURE);
			case 0:
				execvp(args[0], args);
				printf("Программа не найденна\n");
				_exit(EXIT_FAILURE);
			default:
				wait(&rv);
		}
	}

	exit(EXIT_SUCCESS);
}