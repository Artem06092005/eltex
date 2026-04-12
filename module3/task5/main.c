#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int fd = -1;
volatile sig_atomic_t sigint_count = 0;

void sigHandler(int sig) {
	if (sig == SIGINT) {
		sigint_count++;
		if (sigint_count < 3) {
			const char msg[] = "\nПолучен сигнал SIGINT\n";
			write(fd, msg, sizeof(msg) - 1);
		} else {
			const char msg[] = "\nПолучен 3-й SIGINT. Завершение...\n";
			write(fd, msg, sizeof(msg) - 1);
			close(fd);
			exit(EXIT_SUCCESS);
		}
	} else if (sig == SIGQUIT) {
		const char msg[] = "\nПолучен сигнал SIGQUIT\n";
		write(fd, msg, sizeof(msg) - 1);
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

	while (1) {
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