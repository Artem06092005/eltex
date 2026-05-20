#define _GNU_SOURCE

#include "driverRun.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void driverRun(int msqid) {
	driverStatus status = STATUS_AVAILABLE;
	int timeLeft = 0;
	time_t startTime = 0;

	int tfd = timerfd_create(CLOCK_MONOTONIC, 0);

	fd_set drvMaster, drvFds;
	FD_ZERO(&drvMaster);
	FD_ZERO(&drvFds);
	FD_SET(tfd, &drvMaster);
	int fdmax = tfd;
	struct msgbuff msg;

	while (1) {
		struct timeval drvTv = {0, 100000};
		drvFds = drvMaster;
		if (select(fdmax + 1, &drvFds, NULL, NULL, &drvTv) < 0) {
			perror("select()\n");
			_exit(EXIT_FAILURE);
		}
		if (FD_ISSET(tfd, &drvFds)) {
			u_int64_t exp;
			read(tfd, &exp, sizeof(exp));
			status = STATUS_AVAILABLE;
			timeLeft = 0;
		}

		if (msgrcv(msqid, &msg, sizeof(msg), getpid(), IPC_NOWAIT) > 0) {
			if (msg.type == MSG_NEW_TASK) {
				if (status == STATUS_BUSY) {
					int elapsed = (int)(time(NULL) - startTime);
					int remaining = timeLeft - elapsed;
					timeLeft = (remaining > 0 ? remaining : 0) + msg.taskTimer;
				} else {
					status = STATUS_BUSY;
					timeLeft = msg.taskTimer;
				}
				startTime = time(NULL);

				struct itimerspec ts = {.it_value.tv_sec = timeLeft};
				timerfd_settime(tfd, 0, &ts, NULL);
			} else if (msg.type == MSG_STATUS_REQ) {
				int elapsed = (int)(time(NULL) - startTime);
				int remaining = timeLeft - elapsed;

				msg.mtype = getppid();
				msg.status = status;
				msg.type = MSG_STATUS_RESP;
				msg.timeLeft = remaining > 0 ? remaining : 0;
				msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);
			}
		}
	}
	_exit(EXIT_SUCCESS);
}