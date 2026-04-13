#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#include "data.h"

char connect_msg[] = "Connect to server";

int main(int argc, char* argv[]) {
	if (argc < 2) {
		perror("Укажите номер клиента в аргументах запуска\n");
		exit(EXIT_FAILURE);
	}
	long priority = 10 * (atol(argv[1]) + 1);

	key_t key = ftok("server.c", 1);
	int msqid = msgget(key, 0);
	if (msqid == -1) {
		perror("msgget\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	switch (pid = fork()) {
		case -1:
			perror("fork\n");
			exit(EXIT_FAILURE);
		case 0: {
			while (1) {
				if (msgrcv(msqid, &msgbuf,
						   sizeof(msgbuf) - sizeof(msgbuf.mtype), priority,
						   0) == -1) {
					perror("msgrcv\n");
					_exit(EXIT_FAILURE);
				}
				printf("\n[Клиент %ld]: %s\n", msgbuf.sender_id, msgbuf.mtext);
				fflush(stdout);
			}
			break;
		}
		default: {
			msgbuf.mtype = server_priority;
			msgbuf.sender_id = priority;
			strcpy(msgbuf.mtext, "Connect to Server");
			if (msgsnd(msqid, &msgbuf, sizeof(msgbuf) - sizeof(msgbuf.mtype),
					   0) == -1) {
				perror("msgsnd");
				exit(EXIT_FAILURE);
			}
			printf("Вы вошли как клиент %ld. Для выхода введите 'shutdown'.\n",
				   priority);

			while (1) {
				if (fgets(msgbuf.mtext, size_text, stdin) == NULL) break;

				msgbuf.mtext[strcspn(msgbuf.mtext, "\n")] = '\0';

				msgbuf.mtype = server_priority;
				msgbuf.sender_id = priority;

				if (msgsnd(msqid, &msgbuf,
						   sizeof(msgbuf) - sizeof(msgbuf.mtype), 0) == -1) {
					perror("msgsnd");
					exit(EXIT_FAILURE);
				}

				if (strncmp(msgbuf.mtext, "shutdown", 8) == 0) {
					break;
				}
			}
			kill(pid, SIGKILL);
		}
	}
	return 0;
}