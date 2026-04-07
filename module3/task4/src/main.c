#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"

int main() {
	char buffer[bufferSize];
	char *comm[CommSize];

	while (1) {
		memset(buffer, 0, bufferSize);
		memset(comm, 0, sizeof(comm));

		printf("----------------------------------------\n");
		printf("Введите программу и аргументы запуска\n");
		printf("----------------------------------------\n");
		fgets(buffer, sizeof(buffer), stdin);

		buffer[strcspn(buffer, "\n")] = '\0';

		if (strcmp(buffer, "exit") == 0) break;

		size_t countComm = splitComm(buffer, comm);

		if (countComm == 0)
			continue;
		else {
			execute_pipeline(comm, 0, countComm, -1);
		}
	}

	exit(EXIT_SUCCESS);
}