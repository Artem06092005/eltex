#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "LinkedList.h"
#include "dispatcher.h"

volatile sig_atomic_t keepRunning = 1;
void sigHandler(int sg) { keepRunning = 0; }

int main() {
	printf(
		"Команды:\ncreate_driver\nsend_task <pid> <task_timer>\nget_status "
		"<pid>\nget_drivers\n--------------------------------------------\n");

	fd_set master, read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	FD_SET(STDIN_FILENO, &master);
	int fd_max = STDIN_FILENO;

	key_t key = ftok("main.c", 1);
	int msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
	if (msqid == -1) {
		perror("msgget\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sigHandler);

	LinkedList* driverList = initLinkedList();
	while (keepRunning) {
		struct timeval tv = {0, 100000};
		read_fds = master;

		if (select(fd_max + 1, &read_fds, NULL, NULL, &tv) < 0) {
			msgctl(msqid, IPC_RMID, NULL);
			perror("select");
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(STDIN_FILENO, &read_fds)) {
			char buff[256];
			fgets(buff, sizeof(buff), stdin);
			char cmd[64];
			sscanf(buff, "%s", cmd);

			if (strcmp(cmd, "create_driver") == 0)
				cmd_create_driver(driverList, msqid);
			else if (strcmp(cmd, "send_task") == 0)
				cmd_send_task(driverList, msqid, buff);
			else if (strcmp(cmd, "get_status") == 0)
				cmd_get_status(driverList, msqid, buff);
			else if (strcmp(cmd, "get_drivers") == 0)
				cmd_get_drivers(driverList, msqid);
			else
				printf("Command not found\n");
		}
	}
	Node* p = driverList->head;
	while (p != NULL) {
		kill(p->data.pid, SIGTERM);
		p = p->next;
	}
	msgctl(msqid, IPC_RMID, NULL);
	freeLinkedList(driverList);
}