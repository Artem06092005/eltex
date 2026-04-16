#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM_COUNT 0
#define SEM_MUTEX 1

volatile sig_atomic_t keep_running = 1;
void sigHandler(int sig) { keep_running = 0; }

int semid;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Использование: %s <файл_данных>\n", argv[0]);
		exit(1);
	}

	char *data_file = argv[1];
	char offset_file[256];
	sprintf(offset_file, "%s.offset", data_file);

	key_t key = ftok(data_file, 'A');
	semid = semget(key, 2, 0666);
	if (semid == -1) {
		perror("Семафоры не найдены. Сначала запустите производителя");
		exit(1);
	}

	char line[1024];
	while (keep_running) {
		struct sembuf lock_pair[] = {{SEM_COUNT, -1, SEM_UNDO},
									 {SEM_MUTEX, -1, SEM_UNDO}};
		printf("[Потребитель %d] Ожидание данных...\n", getpid());
		semop(semid, lock_pair, 2);

		int current_offset = 0;
		FILE *of = fopen(offset_file, "r+");
		if (!of) of = fopen(offset_file, "w+");
		fscanf(of, "%d", &current_offset);

		FILE *df = fopen(data_file, "r");
		int read_success = 0;
		for (int i = 0; i <= current_offset; i++) {
			if (!fgets(line, sizeof(line), df)) {
				read_success = 0;
				break;
			}
			read_success = 1;
		}
		fclose(df);
		if (!read_success) {
			struct sembuf unlock = {SEM_MUTEX, 1, SEM_UNDO};
			semop(semid, &unlock, 1);
			continue;
		}
		int min = INT_MAX, max = INT_MIN, num, found = 0;
		char *token = strtok(line, " \n");
		while (token) {
			num = atoi(token);
			if (num < min) min = num;
			if (num > max) max = num;
			found = 1;
			token = strtok(NULL, " \n");
		}

		if (found) {
			printf("[Потребитель %d] Строка %d: Min = %d, Max = %d\n", getpid(),
				   current_offset + 1, min, max);
		}

		rewind(of);
		fprintf(of, "%d", current_offset + 1);
		fclose(of);

		struct sembuf unlock = {SEM_MUTEX, 1, SEM_UNDO};
		semop(semid, &unlock, 1);

		sleep(1);
	}
	printf("Завершение работы.\n");
	exit(EXIT_SUCCESS);
}