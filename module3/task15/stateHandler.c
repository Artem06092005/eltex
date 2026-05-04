#include "stateHandler.h"

#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

const char firstMsg[] =
	"Enter № operation:\n1)Sum\n2)Diff\n3)Mul\n4)Div\n5)Send "
	"file\n";
const char par1Msg[] = "Enter param 1:\n";
const char par2Msg[] = "Enter param 2:\n";
const char numErr[] = "Неверное цифра операции\n";
const char askName[] = "Enter remote filename:\n";

int sum(int a, int b) { return a + b; }
int difference(int a, int b) { return a - b; }
int multiplication(int a, int b) { return a * b; }
int division(int a, int b) { return (b != 0) ? a / b : 0; }
typedef int (*myfuncs)(int, int);
myfuncs oper[] = {NULL, sum, difference, multiplication, division};

void menuHandler(int val, clientContext* context) {
	if (val < 1 || val > sizeof(oper) / sizeof(oper[0])) {
		write(context->fd, numErr, strlen(numErr));
		return;
	}
	if (val == 5) {
		context->state = STATE_RECV_FILE;
		write(context->fd, askName, strlen(askName));
		return;
	}
	context->operation = val;
	context->state = STATE_PAR1;
	write(context->fd, par1Msg, strlen(par1Msg));
}

void par1Handler(int val, clientContext* context) {
	context->a = val;
	context->state = STATE_PAR2;
	write(context->fd, par2Msg, strlen(par2Msg));
}

void par2Handler(int val, clientContext* context) {
	if (context->operation > 0 &&
		context->operation < sizeof(oper) / sizeof(oper[0])) {
		char fullMsg[BUF_SIZE * 2];
		sprintf(fullMsg, "Res: %d\n%s",
				oper[context->operation](context->a, val), firstMsg);
		write(context->fd, fullMsg, strlen(fullMsg));
		context->state = STATE_MENU;
	}
}

void recvFileHandler(clientContext* context, char* buff) {
	char* filename = buff;
	filename[strcspn(filename, "\n")] = '\0';
	printf("Receiving file: %s\n", filename);

	strncpy(context->fileName, filename, sizeof(context->fileName) - 1);
	context->fileFd =
		open(context->fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (context->fileFd < 0) {
		const char* errMsg = "Cannot create file\n";
		write(context->fd, errMsg, strlen(errMsg));
		context->state = STATE_MENU;
		write(context->fd, firstMsg, strlen(firstMsg));

	} else {
		const char* ready = "READY\n";
		write(context->fd, ready, strlen(ready));
		context->state = STATE_FILE_DATA;
	}
}

void fileDataHandler(int* n, clientContext* context, char* buff, int epfd) {
	while (1) {
		if (*n >= 4 && strncmp(buff + *n - 4, "DONE", 4) == 0) {
			if (*n > 4) {
				write(context->fileFd, buff, *n - 4);
			}

			close(context->fileFd);
			context->fileFd = -1;
			context->state = STATE_MENU;

			write(context->fd, "File received\n", 14);
			write(context->fd, firstMsg, strlen(firstMsg));
			return;
		} else {
			if (*n > 0) {
				write(context->fileFd, buff, *n);
			}
		}
		*n = recv(context->fd, buff, sizeof(buff), 0);

		if (*n < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			}
			perror("recv file error");
			return;
		}
		if (*n == 0) {
			printf(
				"Connection closed during file "
				"transfer\n");
			close(context->fileFd);
			context->fileFd = -1;
			epoll_ctl(epfd, EPOLL_CTL_DEL, context->fd, NULL);
			close(context->fd);
			free(context);
			return;
		}
	}
}