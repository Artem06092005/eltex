#pragma once

enum { size_text = 50, server_priority = 10 };

struct msgbuf {
	long mtype;
	long sender_id;
	char mtext[size_text];
} msgbuf;