#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

char sem_name_parent[] = "/my_sem_p";
char sem_name_child[] = "/my_sem_c";

enum { sizeBuff = 32, maxQuantityRand = 50, max_rand = 32768 };

typedef struct {
	int count;
	int numbers[maxQuantityRand];
	int min;
	int max;
} SharedData;

int proc_quan = 0;
volatile sig_atomic_t keep_running = 1;

void sigHandler(int sg) { keep_running = 0; }

int main() {
	signal(SIGINT, sigHandler);

	srand(time(NULL));

	int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0664);
	if (shmid == -1) {
		perror("shmget()");
		exit(EXIT_FAILURE);
	}
	SharedData *sharedData = shmat(shmid, NULL, 0);
	if (sharedData == (void *)-1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	sem_t *sem_p = sem_open(sem_name_parent, O_CREAT, 0660, 1);
	sem_t *sem_c = sem_open(sem_name_child, O_CREAT, 0660, 0);

	pid_t pid = fork();

	printf("Ctrl+C - выход\n");

	if (pid < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		while (1) {
			sem_wait(sem_c);

			int min, max, first = 1;
			for (int i = 0; i < sharedData->count; i++) {
				if (first) {
					min = max = sharedData->numbers[i];
					first = 0;
					continue;
				}
				if (sharedData->numbers[i] < min) min = sharedData->numbers[i];
				if (sharedData->numbers[i] > max) max = sharedData->numbers[i];
			}
			sharedData->min = min;
			sharedData->max = max;

			sem_post(sem_p);
		}
	} else {
		while (keep_running) {
			sem_wait(sem_p);

			sharedData->count = rand() % maxQuantityRand + 1;
			for (int i = 0; i < sharedData->count; i++) {
				sharedData->numbers[i] = rand() % max_rand - max_rand / 2;
			}
			sem_post(sem_c);
			sem_wait(sem_p);

			printf("Набор данных #%d :Min: %d  ; Max: %d Всего чисел:%d\n",
				   ++proc_quan, sharedData->min, sharedData->max,
				   sharedData->count);
			sem_post(sem_p);
		}

		printf("Количество обработанных данных - %d\n", proc_quan);
		kill(pid, SIGKILL);

		if (shmdt(sharedData) == -1) {
			perror("shmdt");
			exit(EXIT_FAILURE);
		}
		if (shmctl(shmid, IPC_RMID, NULL) == -1) {
			perror("shmctl");
			exit(EXIT_FAILURE);
		}

		sem_close(sem_p);
		sem_close(sem_c);
		sem_unlink(sem_name_parent);
		sem_unlink(sem_name_child);

		exit(EXIT_SUCCESS);
	}
}