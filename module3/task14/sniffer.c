#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 50000

void dump(int fd, const unsigned char *buff, int len) {
	char line[64];
	int pos = 0;

	for (int i = 0; i < len; i++) {
		pos += snprintf(line + pos, sizeof(line) - pos, "%02x ", buff[i]);
		if ((i + 1) % 16 == 0) {
			line[pos - 1] = '\n';
			write(fd, line, pos);
			pos = 0;
		}
	}
	if (pos > 0) {
		line[pos - 1] = '\n';
		write(fd, line, pos);
	}
}

int is_this_port(const unsigned char *buff, int len) {
	struct iphdr *ipHdr = (struct iphdr *)buff;
	unsigned int ipHdrLen = ipHdr->ihl * 4;

	if (len < ipHdrLen + sizeof(struct udphdr)) return 0;

	struct udphdr *udphdr = (struct udphdr *)(buff + ipHdrLen);
	uint16_t srcPort = ntohs(udphdr->source);
	uint16_t dstPort = ntohs(udphdr->dest);

	return (srcPort == SERVER_PORT || dstPort == SERVER_PORT);
}

void printInfo(const unsigned char *buff, int len) {
	struct iphdr *ipHdr = (struct iphdr *)buff;
	printf("Ip заголовок:\n---------------\n");

	struct in_addr src, dst;
	src.s_addr = ipHdr->addrs.saddr;
	dst.s_addr = ipHdr->addrs.daddr;
	printf("Идентификатор пакета: 0x%04x (%d)\n", ntohs(ipHdr->id),
		   ntohs(ipHdr->id));
	printf("Адресс отправителя:%s\nАдресс получателя:%s\n", inet_ntoa(src),
		   inet_ntoa(dst));

	struct udphdr *udpHdr = (struct udphdr *)(buff + ipHdr->ihl * 4);
	printf("\nUDP заголовок:\n---------------\n");
	printf("Порт отправителя:%d\nПорт получателя:%d\n", ntohs(udpHdr->dest),
		   ntohs(udpHdr->source));

	printf("\nDATA:\n---------------\n%s", buff + ipHdr->ihl * 4 + 8);
}

int main() {
	int sockfd;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addrLen = sizeof(serverAddr);

	if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	printf("Прослушивание порта: %d \n\n", SERVER_PORT);

	while (1) {
		int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
						 (struct sockaddr *)&serverAddr, &addrLen);
		if (n < 0) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		if (!is_this_port(buffer, n)) continue;

		int fd = open("dump", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1) {
			perror("open()");
			close(sockfd);
			exit(EXIT_FAILURE);
		}
		dump(fd, (unsigned char *)buffer, n);
		close(fd);
		printInfo(buffer, n);
	}

	close(sockfd);
}