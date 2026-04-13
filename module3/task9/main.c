#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

char file[] = "file";
char sem_name[] = "my_sem";

enum { sizeBuff = 32, maxQuantityRand = 50, max_rand = 32768 };

int main() {
	sem_t* semid = sem_open(sem_name, O_CREAT, 0660, 0);

	srand(time(NULL));
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(EXIT_FAILURE);

	} else if (pid == 0) {
		sem_wait(semid);

		FILE* fd = fopen(file, "r");
		if (!fd) {
			perror("fopen()");
			_exit(EXIT_FAILURE);
		}

		int num, min, max;
		int first = 1;
		while (fscanf(fd, "%d", &num) == 1) {
			if (first) {
				min = max = num;
				first = 0;
			} else {
				if (num < min) min = num;
				if (num > max) max = num;
			}
		}
		if (!first) {
			printf("Min: %d  ; Max: %d\n", min, max);
		} else {
			printf("Файл пуст\n");
		}
		fclose(fd);
		sem_post(semid);
		sem_close(semid);
		_exit(EXIT_SUCCESS);

	} else {
		FILE* fd = fopen(file, "w");
		if (!fd) {
			perror("fopen()");
			_exit(EXIT_FAILURE);
		}

		int quantityRand = rand() % maxQuantityRand;
		for (int i = 0; i < quantityRand; i++) {
			int num = rand() % max_rand - max_rand / 2;
			fprintf(fd, "%d\n", num);
		}
		fflush(fd);
		fclose(fd);
		sem_post(semid);
		sem_close(semid);
		wait(NULL);
		sem_unlink(sem_name);
		exit(EXIT_SUCCESS);
	}
}