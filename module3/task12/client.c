#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	pid_t pid;
	int sockfd;
	int n;
	char sendline[1000], recvline[1000];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t addrlen;

	bzero((void *)&clientAddr, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(50001);
	if (inet_aton("127.0.0.1", &clientAddr.sin_addr) == 0) {
		perror("inet_aton");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50000);
	if (inet_aton("127.0.0.1", &serverAddr.sin_addr) == 0) {
		perror("inet_aton");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (bind(sockfd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) ==
		-1) {
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if ((pid = fork()) == -1) {
		perror("fork");
	} else if (pid == 0) {
		while (1) {
			socklen_t len = sizeof(serverAddr);
			if ((n = recvfrom(sockfd, recvline, sizeof(recvline) - 1, 0,
							  (struct sockaddr *)&serverAddr, &len)) == -1) {
				perror("recvfrom");
				close(sockfd);
				exit(EXIT_FAILURE);
			}
			recvline[n] = '\0';
			fputs(recvline, stdout);
		}
	} else {
		fputs("Отправте сообщение[client]: ", stdout);
		while (1) {
			fgets(sendline, sizeof(sendline) - 1, stdin);
			if (sendto(sockfd, sendline, strlen(sendline), 0,
					   (struct sockaddr *)&serverAddr,
					   sizeof(serverAddr)) == -1) {
				perror("sendto");
				close(sockfd);
				exit(EXIT_FAILURE);
			}
		}
		close(sockfd);
		exit(EXIT_SUCCESS);
	}
}