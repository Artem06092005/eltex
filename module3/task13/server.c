#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void dostuff(int);

void error(const char* msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int nclients = 0;

void printusers() {
	if (nclients) {
		printf("%d user on-line\n", nclients);
	} else {
		printf("No User on line\n");
	}
}

typedef int (*myfuncs)(int, int);

int sum(int a, int b) { return a + b; }
int difference(int a, int b) { return a - b; }
int multiplication(int a, int b) { return a * b; }
int division(int a, int b) { return (b != 0) ? a / b : 0; }

int main(int argc, char* argv[]) {
	char buff[1024];
	int sockfd, newsockfd;
	int portno;
	int pid;
	socklen_t clilen;
	struct sockaddr_in servAddr, clientAddr;

	printf("TCP SERVER DEMO\n");

	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(EXIT_FAILURE);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("socket()");
	}

	bzero(&servAddr, sizeof(servAddr));
	portno = atoi(argv[1]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portno);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
		error("bind()");
	}

	listen(sockfd, 5);
	clilen = sizeof(clientAddr);

	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clilen);
		if (newsockfd == -1) error("accept()");
		nclients++;

		struct hostent* hst;
		hst = gethostbyaddr((void*)&clientAddr.sin_addr, 4, AF_INET);
		printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "Unknow host",
			   inet_ntoa((struct in_addr)clientAddr.sin_addr));
		printusers();

		pid = fork();
		if (pid < 0) error("fork()");
		if (pid == 0) {
			close(sockfd);
			dostuff(newsockfd);
			_exit(EXIT_SUCCESS);
		} else
			close(newsockfd);
	}
	close(sockfd);
	return 0;
}

void dostuff(int sock) {
	int bytes_recv;
	int a, b;
	char buff[20 * 1024];

#define str0 "Enter № operation:\n1)Sum\n2)Diff\n3)Mul\n4)Div\n5)Send file\n"
#define str1 "Enter 1 parameter\r\n"
#define str2 "Enter 2 parameter\r\n"
#define div0 "Error: division by zero\n"
#define err "Неверное число\n"
#define fErr "ERROR: cannot create file\n"

	myfuncs oper[5] = {NULL, sum, difference, multiplication, division};

	while (1) {
		write(sock, str0, strlen(str0));
		bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
		if (bytes_recv <= 0) break;
		buff[bytes_recv] = '\0';
		if (strncmp(buff, "quit", 4) == 0) break;
		int n = atoi(buff);
		if (n > 0 && n < 5) {
			write(sock, str1, strlen(str1));
			bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
			if (bytes_recv <= 0) break;
			buff[bytes_recv] = '\0';
			if (strncmp(buff, "quit", 4) == 0) break;
			a = atoi(buff);

			write(sock, str2, strlen(str2));
			bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
			if (bytes_recv <= 0) break;
			buff[bytes_recv] = '\0';
			if (strncmp(buff, "quit", 4) == 0) break;
			b = atoi(buff);

			if (n == 4 && b == 0) {
				write(sock, div0, strlen(div0));
			}
			int res = oper[n](a, b);
			sprintf(buff, "%d\n", res);
			write(sock, buff, strlen(buff));
		} else if (n == 5) {
			write(sock, "Enter remote filename:\r\n", 24);
			bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
			if (bytes_recv <= 0) break;
			buff[bytes_recv] = '\0';

			char* filename = buff;
			filename[strcspn(filename, "\n")] = '\0';

			printf("Receiving file: %s\n", filename);

			FILE* file = fopen(filename, "wb");
			if (!file) {
				write(sock, fErr, strlen(fErr));
				continue;
			}

			write(sock, "READY\n", 6);

			int n;
			while ((n = recv(sock, buff, sizeof(buff), 0)) > 0) {
				if (n >= 4 && strncmp(buff + n - 4, "DONE", 4) == 0) {
					fwrite(buff, 1, n - 4, file);
					break;
				}
				fwrite(buff, 1, n, file);
			}
			fclose(file);
			write(sock, "File received\n", 14);
			// break;

		} else {
			write(sock, err, strlen(err));
		}
	}

	nclients--;
	printf("disconnect\n");
	printusers();
}