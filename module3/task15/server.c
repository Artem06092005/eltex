#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "stateHandler.h"

void setNonBlock(int sock) {
	int flags = fcntl(sock, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sock, F_SETFL, flags);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(EXIT_FAILURE);
	}

	int listenSock, epfd;
	struct epoll_event ev, events[MAX_EVENTS];

	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	setNonBlock(listenSock);

	struct sockaddr_in addr = {.sin_family = AF_INET,
							   .sin_port = htons(atoi(argv[1])),
							   .sin_addr.s_addr = INADDR_ANY};

	if (bind(listenSock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(listenSock, 5) == -1) {
		perror("listen()");
		exit(EXIT_FAILURE);
	}

	epfd = epoll_create1(0);
	ev.events = EPOLLIN;
	ev.data.fd = listenSock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenSock, &ev);

	printf("Server start on port %s\n", argv[1]);

	while (1) {
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == listenSock) {
				struct sockaddr_in clientAddr;
				socklen_t len = sizeof(clientAddr);
				int clientSock =
					accept(listenSock, (struct sockaddr*)&clientAddr, &len);

				setNonBlock(clientSock);

				clientContext* contx = calloc(1, sizeof(clientContext));
				contx->fileFd = -1;
				contx->fd = clientSock;
				contx->state = STATE_MENU;

				ev.events = EPOLLIN | EPOLLET;
				ev.data.ptr = contx;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientSock, &ev);

				write(clientSock, firstMsg, strlen(firstMsg));
			} else {
				clientContext* contx = (clientContext*)events[i].data.ptr;
				char buff[BUF_SIZE];
				int n = recv(contx->fd, buff, BUF_SIZE - 1, 0);

				if (n <= 0) {
					printf("Disconnect\n");
					epoll_ctl(epfd, EPOLL_CTL_DEL, contx->fd, NULL);
					close(contx->fd);
					if (contx->fileFd != -1) close(contx->fileFd);
					free(contx);
				} else {
					buff[n] = '\0';
					int val = atoi(buff);
					switch (contx->state) {
						case STATE_MENU:
							menuHandler(val, contx);
							break;
						case STATE_PAR1:
							par1Handler(val, contx);
							break;
						case STATE_PAR2:
							par2Handler(val, contx);
							break;
						case STATE_RECV_FILE: {
							recvFileHandler(contx, buff);
							break;
						}
						case STATE_FILE_DATA: {
							fileDataHandler(&n, contx, buff, epfd);
							break;
						}
					}
				}
			}
		}
	}
}