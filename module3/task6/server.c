#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

enum { size_text = 50, server_priority = 10, quantityClietns = 50 };

int msqid = -1;

struct msgbuf {
	long mtype;
	long sender_id;
	char mtext[size_text];
} msgbuf;

long clients[quantityClietns] = {0};

void sigHandler(int sg) {
	if (msqid != -1) {
		msgctl(msqid, IPC_RMID, NULL);
	}
	exit(EXIT_SUCCESS);
}

int main() {
	key_t key = ftok("server.c", 1);
	msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
	if (msqid == -1) {
		perror("msgget\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sigHandler);

	while (1) {
		if (msgrcv(msqid, &msgbuf, sizeof(msgbuf) - sizeof(long),
				   server_priority, 0) == -1) {
			perror("msgrcv");
			msgctl(msqid, IPC_RMID, NULL);
			exit(EXIT_FAILURE);
		}

		int found = 0;
		int empty_slot = -1;

		for (int i = 0; i < quantityClietns; i++) {
			if (clients[i] == msgbuf.sender_id) {
				found = 1;

				if (strncmp(msgbuf.mtext, "shutdown", 8) == 0) {
					clients[i] = 0;
					printf("Клиент %ld отключен\n", msgbuf.sender_id);
					goto skip_send;
				}
				break;
			}
			if (clients[i] == 0 && empty_slot == -1) empty_slot = i;
		}

		if (!found && empty_slot != -1) {
			clients[empty_slot] = msgbuf.sender_id;
			printf("Новый клиент: %ld\n", msgbuf.sender_id);
		}

		for (int i = 0; i < quantityClietns; i++) {
			if (clients[i] != msgbuf.sender_id && clients[i] != 0) {
				msgbuf.mtype = clients[i];
				if (msgsnd(msqid, &msgbuf, sizeof(msgbuf) - sizeof(long), 0) ==
					-1) {
					perror("msgsnd error");

					exit(EXIT_FAILURE);
				}
			}
		}
	skip_send:
	}
	msgctl(msqid, IPC_RMID, NULL);
}