#pragma once

#include <stddef.h>

enum { CommSize = 50, bufferSize = 300 };

size_t splitComm(char *buffer, char **comm);
void splitArg(char *buffer, char **args);

void execute_pipeline(char **comm, int comm_index, int total_comm,
					  int prev_pipe_read);
void handle_redirections(char **args);