#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"

void handle_redirections(char **args) {
	for (int i = 0; args[i] != NULL; i++) {
		if (strcmp(args[i], ">") == 0) {
			int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0) {
				perror("open >");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}

		else if (strcmp(args[i], ">>") == 0) {
			int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0) {
				perror("open >>");
				exit(EXIT_FAILURE);
			}

			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}

		else if (strcmp(args[i], "<") == 0) {
			int fd = open(args[i + 1], O_RDONLY);
			if (fd < 0) {
				perror("open <");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
		}
	}
}

void execute_pipeline(char **comm, int comm_index, int total_comm,
					  int prev_pipe_read) {
	if (comm_index >= total_comm) return;

	int fd[2];
	int has_next = (comm_index < total_comm - 1);

	if (has_next) {
		if (pipe(fd) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	pid_t pid;
	switch (pid = fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			if (prev_pipe_read != -1) {
				dup2(prev_pipe_read, STDIN_FILENO);
				close(prev_pipe_read);
			}
			if (has_next) {
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}

			char *args[CommSize];
			splitArg(comm[comm_index], args);

			handle_redirections(args);

			execvp(args[0], args);
			fprintf(stderr, "Программа не найденна %s\n", args[0]);
			_exit(EXIT_FAILURE);

		default:
			if (prev_pipe_read != -1) close(prev_pipe_read);
			if (has_next) {
				close(fd[1]);
				execute_pipeline(comm, comm_index + 1, total_comm, fd[0]);
			}
			wait(NULL);
			break;
	}
}