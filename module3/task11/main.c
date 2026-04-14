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

char shm_name[] = "/my_shm";
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

	int shmid = shm_open(shm_name, O_CREAT | O_RDWR, 0660);
	ftruncate(shmid, sizeof(SharedData));
	SharedData *data = (SharedData *)mmap(
		NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

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
			for (int i = 0; i < data->count; i++) {
				if (first) {
					min = max = data->numbers[i];
					first = 0;
					continue;
				}
				if (data->numbers[i] < min) min = data->numbers[i];
				if (data->numbers[i] > max) max = data->numbers[i];
			}
			data->min = min;
			data->max = max;

			sem_post(sem_p);
		}
	} else {
		while (keep_running) {
			sem_wait(sem_p);

			data->count = rand() % maxQuantityRand + 1;
			for (int i = 0; i < data->count; i++) {
				data->numbers[i] = rand() % max_rand - max_rand / 2;
			}
			sem_post(sem_c);
			sem_wait(sem_p);

			printf("Набор данных #%d :Min: %d  ; Max: %d Всего чисел:%d\n",
				   ++proc_quan, data->min, data->max, data->count);
			sem_post(sem_p);
		}

		printf("Количество обработанных данных - %d\n", proc_quan);
		kill(pid, SIGKILL);

		munmap(data, sizeof(SharedData));
		shm_unlink(shm_name);
		sem_close(sem_p);
		sem_close(sem_c);
		sem_unlink(sem_name_parent);
		sem_unlink(sem_name_child);

		exit(EXIT_SUCCESS);
	}
}