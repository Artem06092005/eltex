#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include "headerUDP.h"

volatile sig_atomic_t keep_running = 1;
void sigHandler(int sg) {
	if (sg == SIGINT) {
		keep_running = 0;
	}
}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		fprintf(stderr, "usage %s hostname port serverPort\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int port = atoi(argv[2]);
	if (!(0 < port && port < 65536)) {
		fprintf(stderr, "Incorrect port: %d", port);
		exit(EXIT_FAILURE);
	}
	int serverPort = atoi(argv[3]);
	if (!(0 < serverPort && serverPort < 65536)) {
		fprintf(stderr, "Incorrect port: %d", serverPort);
		exit(EXIT_FAILURE);
	}

	int sockfd;
	int n;
	struct sockaddr_in serverAddr;
	socklen_t socklen = sizeof(serverAddr);
	char buff[BUF_SIZE];

	bzero(&serverAddr, socklen);
	serverAddr.sin_family = PF_INET;
	if (inet_aton(argv[1], &serverAddr.sin_addr) == 0) {
		perror("inet_aton()");
		exit(EXIT_FAILURE);
	}

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	struct sigaction sa;
	sa.sa_handler = sigHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	while (1) {
		printf("Введите сообщение:\n");
		fgets(buff, sizeof(buff), stdin);
		if (!keep_running) break;
		buff[strcspn(buff, "\n")] = '\0';
		if (strlen(buff) == 0) continue;

		if (strcmp(buff, "quit") == 0) break;

		char* msg = addUDPhader(buff, port, serverPort);
		int msgLen = sizeof(struct udphdr) + strlen(buff);
		if ((sendto(sockfd, msg, msgLen, 0, (struct sockaddr*)&serverAddr,
					socklen)) == -1) {
			perror("sendto()");
			close(sockfd);
			exit(EXIT_FAILURE);
		}
		free(msg);

		while (1) {
			if ((n = recv(sockfd, buff, sizeof(buff), 0)) == -1) {
				perror("recv()");
				exit(EXIT_FAILURE);
			}
			buff[n] = '\0';
			struct iphdr* ipHeader = (struct iphdr*)buff;
			struct udphdr* udpHeader =
				(struct udphdr*)(buff + (ipHeader->ihl * 4));
			if (ntohs(udpHeader->dest) == port) {
				printf("Ответ: %s\n",
					   buff + ipHeader->ihl * 4 + sizeof(struct udphdr));
				break;
			}
		}
	}

	char* exitMsg = addUDPhader("EXIT", port, serverPort);
	int exitLen = sizeof(struct udphdr) + strlen("EXIT");
	sendto(sockfd, exitMsg, exitLen, 0, (struct sockaddr*)&serverAddr, socklen);
	free(exitMsg);
	close(sockfd);
	exit(EXIT_SUCCESS);
}