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

#define MAX_EVENTS 64
#define BUF_SIZE 1024

typedef enum {
	STATE_MENU,
	STATE_PAR1,
	STATE_PAR2,
	STATE_RECV_FILE,
	STATE_FILE_DATA
} clientState;

typedef struct {
	int fd;
	clientState state;
	int operation;
	int a;
	int fileFd;
	char fileName[256];
} clientContext;

void setNonBlock(int sock) {
	int flags = fcntl(sock, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sock, F_SETFL, flags);
}

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
					close(contx->fileFd);
					free(contx);
				} else {
					buff[n] = '\0';
					int val = atoi(buff);
					switch (contx->state) {
						case STATE_MENU:
							if (val < 1 ||
								val > sizeof(oper) / sizeof(oper[0])) {
								write(contx->fd, numErr, strlen(numErr));
								break;
							}
							if (val == 5) {
								contx->state = STATE_RECV_FILE;
								write(contx->fd, askName, strlen(askName));
								break;
							}
							contx->operation = val;
							contx->state = STATE_PAR1;
							write(contx->fd, par1Msg, strlen(par1Msg));
							break;
						case STATE_PAR1:
							contx->a = val;
							contx->state = STATE_PAR2;
							write(contx->fd, par2Msg, strlen(par2Msg));
							break;
						case STATE_PAR2:
							if (contx->operation > 0 &&
								contx->operation <
									sizeof(oper) / sizeof(oper[0])) {
								char fullMsg[BUF_SIZE * 2];
								sprintf(fullMsg, "Res: %d\n%s",
										oper[contx->operation](contx->a, val),
										firstMsg);
								write(contx->fd, fullMsg, strlen(fullMsg));
								contx->state = STATE_MENU;
							}
							break;
						case STATE_RECV_FILE: {
							char* filename = buff;
							filename[strcspn(filename, "\n")] = '\0';
							printf("Receiving file: %s\n", filename);

							strncpy(contx->fileName, filename,
									sizeof(contx->fileName) - 1);
							contx->fileFd =
								open(contx->fileName,
									 O_WRONLY | O_CREAT | O_TRUNC, 0644);

							if (contx->fileFd < 0) {
								const char* errMsg = "Cannot create file\n";
								write(contx->fd, errMsg, strlen(errMsg));
								contx->state = STATE_MENU;
								write(contx->fd, firstMsg, strlen(firstMsg));

							} else {
								const char* ready = "READY\n";
								write(contx->fd, ready, strlen(ready));
								contx->state = STATE_FILE_DATA;
							}
							break;
						}
						case STATE_FILE_DATA: {
							while (1) {
								if (n >= 4 &&
									strncmp(buff + n - 4, "DONE", 4) == 0) {
									if (n > 4) {
										write(contx->fileFd, buff, n - 4);
									}

									close(contx->fileFd);
									contx->fileFd = -1;
									contx->state = STATE_MENU;

									write(contx->fd, "File received\n", 14);
									write(contx->fd, firstMsg,
										  strlen(firstMsg));
									break;
								} else {
									if (n > 0) {
										write(contx->fileFd, buff, n);
									}
								}
								n = recv(contx->fd, buff, sizeof(buff), 0);

								if (n < 0) {
									if (errno == EAGAIN ||
										errno == EWOULDBLOCK) {
										break;
									}
									perror("recv file error");
									break;
								}
								if (n == 0) {
									printf(
										"Connection closed during file "
										"transfer\n");
									close(contx->fileFd);
									contx->fileFd = -1;
									epoll_ctl(epfd, EPOLL_CTL_DEL, contx->fd,
											  NULL);
									close(contx->fd);
									free(contx);
									return 0;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}