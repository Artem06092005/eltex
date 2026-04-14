#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

char file[] = "file";
char sem_name_parent[] = "/sem_p";
char sem_name_child[] = "/sem_c";

enum { sizeBuff = 32, maxQuantityRand = 50, max_rand = 32768 };

int keep_running = 1;
void sigHandler(int sg) { keep_running = 0; }

int main() {
	signal(SIGINT, sigHandler);
	sem_t* sem_p = sem_open(sem_name_parent, O_CREAT, 0660, 1);
	sem_t* sem_c = sem_open(sem_name_child, O_CREAT, 0660, 0);

	srand(time(NULL));
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(EXIT_FAILURE);

	} else if (pid == 0) {
		while (1) {
			sem_wait(sem_c);
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
			sem_post(sem_p);
		}
	} else {
		while (keep_running) {
			sem_wait(sem_p);
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
			sem_post(sem_c);
		}
		printf("Конец\n");
		kill(pid, SIGKILL);
		sem_close(sem_p);
		sem_close(sem_c);
		sem_unlink(sem_name_parent);
		sem_unlink(sem_name_child);
		exit(EXIT_SUCCESS);
	}
}