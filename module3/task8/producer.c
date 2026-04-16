#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#define SEM_COUNT 0
#define SEM_MUTEX 1

volatile sig_atomic_t keep_running = 1;

void sigHandler(int sig) { keep_running = 0; }

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Использование: %s <файл_данных>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *data_file = argv[1];
	key_t key = ftok(data_file, 'A');

	signal(SIGINT, sigHandler);

	int semid = semget(key, 2, IPC_CREAT | 0666);

	semctl(semid, SEM_MUTEX, SETVAL, 1);
	semctl(semid, SEM_COUNT, SETVAL, 0);

	srand(time(NULL));

	while (keep_running) {
		struct sembuf lock = {SEM_MUTEX, -1, SEM_UNDO};
		semop(semid, &lock, 1);

		FILE *f = fopen(data_file, "a");
		if (f) {
			int n = rand() % 10 + 1;
			printf("[Производитель %d] Пишу %d чисел в %s\n", getpid(), n,
				   data_file);
			for (int i = 0; i < n; i++) {
				fprintf(f, "%d ", rand() % 1000);
			}
			fprintf(f, "\n");
			fclose(f);
		}

		struct sembuf unlock_and_inc[] = {{SEM_MUTEX, 1, SEM_UNDO},
										  {SEM_COUNT, 1, 0}};
		semop(semid, unlock_and_inc, 2);

		sleep(rand() % 5 + 1);
	}
	printf("Конец\n");
	semctl(semid, 0, IPC_RMID);
	exit(EXIT_SUCCESS);
}