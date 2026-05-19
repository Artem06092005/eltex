#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <unistd.h>

typedef enum {
	MSG_NEW_TASK,
	MSG_STATUS_REQ,	 // get rider status
	MSG_STATUS_RESP	 // response
} msgType;

struct msgbuff {
	long mtype;
	msgType type;
	int taskTimer;
	int status;
	int timeLeft;

} msgbuf;

volatile sig_atomic_t keepRunning = 1;
void sigHandler(int sg) { keepRunning = 0; }

int main() {
	printf(
		"Команды:\ncreate_driver\nsend_task <pid> <task_timer>\nget_status "
		"<pid>\nget_drivers\n");

	fd_set master, read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	FD_SET(STDIN_FILENO, &master);
	int fd_max = STDIN_FILENO;

	struct timeval tv = {0, 100000};

	key_t key = ftok("main.c", 1);
	int msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
	if (msqid == -1) {
		perror("msgget\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sigHandler);

	while (keepRunning) {
		read_fds = master;

		if (select(fd_max + 1, &read_fds, NULL, NULL, &tv) < 0) {
			msgctl(msqid, IPC_RMID, NULL);
			perror("select");
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(STDIN_FILENO, &read_fds)) {
			char buff[256];
			fgets(buff, sizeof(buff), stdin);
			printf("Получена команда %s", buff);
			char cmd[64];
			sscanf(buff, "%s", cmd);

			if (strcmp(cmd, "create_driver") == 0) {
				// create_driver
			} else if (strcmp(cmd, "send_task") == 0) {
				// send_task
			} else if (strcmp(cmd, "get_status") == 0) {
				// get_status
			} else if (strcmp(cmd, "nget_drivers") == 0) {
				// get_drivers
			} else {
				printf("Command not found\n");
			}
		}

		msgrcv(msqid, &msgbuf, sizeof(msgbuf), 0, IPC_NOWAIT);
	}
	msgctl(msqid, IPC_RMID, NULL);
}