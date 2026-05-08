#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "LinkedList.h"
#include "headerUDP.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int port = atoi(argv[1]);
	if (!(0 < port && port < 65536)) {
		fprintf(stderr, "Incorrect port: %d", port);
		exit(EXIT_FAILURE);
	}
	int n;
	int sockfd;
	LinkedList* clients = initLinkedList();
	char buff[BUF_SIZE];

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	while (1) {
		if ((n = recv(sockfd, buff, sizeof(buff), 0)) == -1) {
			perror("recv()");
			exit(EXIT_FAILURE);
		}
		buff[n] = '\0';

		struct iphdr* ipHeader = (struct iphdr*)buff;
		struct udphdr* udpHeader = (struct udphdr*)(buff + (ipHeader->ihl * 4));

		if (ntohs(udpHeader->dest) == port) {
			Node* client = findClient(clients, ntohl(ipHeader->saddr),
									  ntohs(udpHeader->source));
			if (!client) {
				client = addClient(clients, ntohl(ipHeader->saddr),
								   ntohs(udpHeader->source));
			}

			if (strcmp((buff + sizeof(struct udphdr) + (ipHeader->ihl * 4)),
					   "EXIT") == 0) {
				client->data.counter = 1;
				continue;
			}

			char buff2[BUF_SIZE + 3];
			sprintf(buff2, "%s %d", (buff + 8 + (ipHeader->ihl * 4)),
					client->data.counter);

			char* sndMsg = addUDPhader(buff2, port, ntohs(udpHeader->source));
			int msgLen = sizeof(struct udphdr) + strlen(buff2);
			struct sockaddr_in clientAddr;
			clientAddr.sin_family = PF_INET;
			clientAddr.sin_addr.s_addr = ipHeader->saddr;

			if ((sendto(sockfd, sndMsg, msgLen, 0,
						(struct sockaddr*)&clientAddr, sizeof(clientAddr))) ==
				-1) {
				perror("sendto()");
				close(sockfd);
				exit(EXIT_FAILURE);
			}
			client->data.counter++;
		}
	}

	freeLinkedList(clients);
	close(sockfd);
	exit(EXIT_SUCCESS);
}