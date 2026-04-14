#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int fd = -1;
volatile sig_atomic_t sigint_flag = 0;
volatile sig_atomic_t sigquit_flag = 0;

int sigint_count = 0;

void sigHandler(int sig) {
	if (sig == SIGINT) {
		sigint_flag = 1;
	} else if (sig == SIGQUIT) {
		sigquit_flag = 1;
	}
}

int main() {
	fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		perror("Error: open()");
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, sigHandler);
	signal(SIGQUIT, sigHandler);

	int counter = 1;
	char buffer[64];

	const char msg1[] = "\nПолучен сигнал SIGINT\n";
	const char msg2[] = "\nПолучен 3-й SIGINT. Завершение...\n";
	const char msg3[] = "\nПолучен сигнал SIGQUIT\n";
	while (1) {
		if (sigint_flag) {
			sigint_count++;
			if (sigint_count < 3) {
				write(fd, msg1, sizeof(msg1) - 1);
			} else {
				write(fd, msg2, sizeof(msg2) - 1);
				close(fd);
				exit(EXIT_SUCCESS);
			}
			sigint_flag = 0;
		}

		if (sigquit_flag) {
			write(fd, msg3, sizeof(msg3) - 1);
			sigquit_flag = 0;
		}

		int len = sprintf(buffer, "%d\n", counter++);
		if (write(fd, buffer, len) == -1) {
			perror("Error: write()");
			close(fd);
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}