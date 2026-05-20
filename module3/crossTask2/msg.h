#pragma once

typedef enum {
	MSG_NEW_TASK,
	MSG_STATUS_REQ,	 // get rider status
	MSG_STATUS_RESP	 // response
} msgType;

typedef enum { STATUS_BUSY, STATUS_AVAILABLE } driverStatus;

struct msgbuff {
	long mtype;
	msgType type;
	int taskTimer;
	driverStatus status;
	int timeLeft;
};