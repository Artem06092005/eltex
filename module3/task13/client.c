#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

void error(const char* msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void send_file(int sock, const char* local_path) {
	FILE* file = fopen(local_path, "rb");
	if (!file) {
		printf("Ошибка открытия файла.\n");
		close(sock);
		exit(1);
	}

	char buffer[1024];
	int n;
	while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		if (write(sock, buffer, n) < 0) {
			perror("Ошибка отправки файла");
			break;
		}
	}
	fclose(file);

	write(sock, "DONE", 4);
}

int main(int argc, char* argv[]) {
	int my_sock, portno, n;
	struct sockaddr_in serverAddr;
	socklen_t servlen;
	struct hostent* server;
	char buff[1024];

	printf("TCP DEMO CLIENT\n");

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	portno = atoi(argv[2]);

	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock == -1) {
		error("socket()");
	}

	server = gethostbyname(argv[1]);
	if (!server) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(EXIT_FAILURE);
	}

	bzero((void*)&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
	bcopy(server->h_addr_list[0], &serverAddr.sin_addr, server->h_length);

	if (connect(my_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) ==
		-1) {
		error("connect()");
	}

	servlen = sizeof(serverAddr);
	while ((n = recv(my_sock, buff, sizeof(buff) - 1, 0)) > 0) {
		buff[n] = '\0';

		printf("S=>C: %s", buff);
		printf("S<=C: ");
		fgets(buff, sizeof(buff) - 1, stdin);

		if (!strcmp(buff, "quit\n")) {
			printf("Exit...\n");
			close(my_sock);
			exit(EXIT_SUCCESS);
		}

		if (!strcmp(buff, "5\n")) {
			send(my_sock, buff, strlen(buff), 0);

			n = recv(my_sock, buff, sizeof(buff) - 1, 0);
			if (n <= 0) break;
			buff[n] = '\0';

			printf("S=>C: %s", buff);
			printf("S<=C: ");

			fgets(buff, sizeof(buff) - 1, stdin);
			buff[strcspn(buff, "\n")] = '\0';
			char remote_name[256];
			send(my_sock, buff, strlen(buff), 0);

			n = recv(my_sock, buff, sizeof(buff) - 1, 0);
			if (n <= 0) break;
			buff[n] = '\0';

			if (strncmp(buff, "READY", 5) != 0) {
				printf("Server error: %s\n", buff);
				continue;
			}

			printf("Enter local file path to send: ");
			fgets(buff, sizeof(buff) - 1, stdin);
			buff[strcspn(buff, "\n")] = '\0';
			send_file(my_sock, buff);
			continue;
		}

		send(my_sock, buff, strlen(buff), 0);
	}
	if (n == 0) {
		printf("Server closed connection.\n");
	} else if (n == -1) {
		perror("recv");
	}
	close(my_sock);
	exit(EXIT_SUCCESS);
}