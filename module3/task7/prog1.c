#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_NAME_INP "/my_queue1"
#define QUEUE_NAME_OUT "/my_queue2"
#define SIZE 256

int main() {
	mqd_t mqInp, mqOut;
	char buffer[SIZE];
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = 1;
	attr.mq_msgsize = SIZE;

	if ((mqInp = mq_open(QUEUE_NAME_INP, O_CREAT | O_RDWR, 0660, &attr)) ==
		(mqd_t)-1) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	if ((mqOut = mq_open(QUEUE_NAME_OUT, O_CREAT | O_RDWR, 0660, &attr)) ==
		(mqd_t)-1) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}

	while (1) {
		printf("Ввод (программа 1):");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strncmp(buffer, "exit", 4) == 0) {
			mq_send(mqInp, buffer, sizeof(buffer), 255);
			break;
		}

		mq_send(mqInp, buffer, sizeof(buffer), 1);

		printf("Ожидание ответа\n");
		unsigned int prio;
		mq_receive(mqOut, buffer, sizeof(buffer), &prio);

		if (prio == 255) {
			printf("Программа 2 завершила процесс\n");
			break;
		}
		printf("Ответ программы 2: %s\n", buffer);
	}

	mq_close(mqInp);
	mq_close(mqOut);
	mq_unlink(QUEUE_NAME_INP);
	mq_unlink(QUEUE_NAME_OUT);

	exit(EXIT_SUCCESS);
}