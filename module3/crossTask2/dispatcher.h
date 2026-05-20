#pragma once
#include "LinkedList.h"
#include "msg.h"

void cmd_create_driver(LinkedList* list, int msqid);
void cmd_send_task(LinkedList* list, int msqid, char* buff);
void cmd_get_status(LinkedList* list, int msqid, char* buff);
void cmd_get_drivers(LinkedList* list, int msqid);
