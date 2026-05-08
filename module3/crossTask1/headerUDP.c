#include "headerUDP.h"

#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char* addUDPhader(char* buff, int sourcePort, int destPort) {
	struct udphdr head = {.source = htons(sourcePort),
						  .dest = htons(destPort),
						  .len = htons(strlen(buff) + sizeof(struct udphdr)),
						  .check = 0};
	char* packet = malloc(strlen(buff) + sizeof(struct udphdr));
	memcpy(packet, &head, sizeof(struct udphdr));
	memcpy(packet + sizeof(struct udphdr), buff, strlen(buff));
	return packet;
}