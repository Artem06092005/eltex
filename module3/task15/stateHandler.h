#pragma once

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX_EVENTS 64
#define BUF_SIZE 1024

typedef enum {
	STATE_MENU,
	STATE_PAR1,
	STATE_PAR2,
	STATE_RECV_FILE,
	STATE_FILE_DATA
} clientState;

extern const char firstMsg[];
extern const char par1Msg[];
extern const char par2Msg[];
extern const char numErr[];
extern const char askName[];

typedef struct {
	int fd;
	clientState state;
	int operation;
	int a;
	int fileFd;
	char fileName[256];
} clientContext;

void menuHandler(int val, clientContext* context);
void par1Handler(int val, clientContext* context);
void par2Handler(int val, clientContext* context);
void recvFileHandler(clientContext* context, char* buff);
void fileDataHandler(int* n, clientContext* context, char* buff, int epfd);