#define _GNU_SOURCE

#include "dispatcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#include "driverRun.h"

void cmd_create_driver(LinkedList* list, int msqid) {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		driverRun(msqid);
	} else {
		addDriver(list, pid);
		printf("Driver created, PID: %d\n", pid);
	}
}

void cmd_send_task(LinkedList* list, int msqid, char* buff) {
	pid_t pid;
	int timer;
	char cmd[64];
	if (sscanf(buff, "%s %d %d", cmd, &pid, &timer) != 3) {
		printf("Usage: send_task <pid> <task_timer>\n");
		return;
	}
	if (findDriver(list, pid) == NULL) {
		printf("Driver %d not found\n", pid);
	} else {
		struct msgbuff msg;
		msg.mtype = pid;
		msg.type = MSG_NEW_TASK;
		msg.taskTimer = timer;
		msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);
		printf("Task sent to driver %d, busy for %d sec\n", pid, timer);
	}
}

void cmd_get_status(LinkedList* list, int msqid, char* buff) {
	pid_t pid;
	char cmd[64];
	if (sscanf(buff, "%s %d", cmd, &pid) != 2) {
		printf("Usage: get_status <pid>\n");
		return;
	}

	if (findDriver(list, pid) == NULL) {
		printf("Driver %d not found\n", pid);
	} else {
		struct msgbuff msg;
		msg.mtype = pid;
		msg.type = MSG_STATUS_REQ;
		msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);

		struct msgbuff resp;
		msgrcv(msqid, &resp, sizeof(resp) - sizeof(resp.mtype), getpid(), 0);

		if (resp.status == STATUS_BUSY) {
			printf("Driver %d: Busy <%d>\n", pid, resp.timeLeft);
		} else {
			printf("Driver %d: Available\n", pid);
		}
	}
}

void cmd_get_drivers(LinkedList* list, int msqid) {
	Node* p = list->head;
	if (p == NULL) {
		printf("No drivers\n");
	}
	while (p != NULL) {
		struct msgbuff msg;
		msg.mtype = p->data.pid;
		msg.type = MSG_STATUS_REQ;
		msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);

		struct msgbuff resp;
		msgrcv(msqid, &resp, sizeof(resp) - sizeof(resp.mtype), getpid(), 0);

		if (resp.status == STATUS_BUSY) {
			printf("PID: %d  Busy <%d>\n", p->data.pid, resp.timeLeft);
		} else {
			printf("PID: %d  Available\n", p->data.pid);
		}
		p = p->next;
	}
}