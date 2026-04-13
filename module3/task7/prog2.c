#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_NAME_INP "/my_queue2"
#define QUEUE_NAME_OUT "/my_queue1"
#define SIZE 256

int main() {
	mqd_t mqInp, mqOut;
	char buffer[SIZE];

	if ((mqInp = mq_open(QUEUE_NAME_INP, O_RDWR)) == (mqd_t)-1) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	if ((mqOut = mq_open(QUEUE_NAME_OUT, O_RDWR)) == (mqd_t)-1) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}

	while (1) {
		printf("Ожидание ответа от программы 1 \n");
		unsigned int prio;
		mq_receive(mqOut, buffer, sizeof(buffer), &prio);

		if (prio == 255) {
			printf("Программа 1 завершила процесс\n");
			break;
		}
		printf("Ответ программы 1: %s\n", buffer);

		printf("Ввод (программа 2):");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strncmp(buffer, "exit", 4) == 0) {
			mq_send(mqInp, buffer, sizeof(buffer), 255);
			break;
		}

		mq_send(mqInp, buffer, sizeof(buffer), 1);
	}

	mq_close(mqInp);
	mq_close(mqOut);
	exit(EXIT_SUCCESS);
}