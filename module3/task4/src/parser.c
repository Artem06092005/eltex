#include "parser.h"

#include <string.h>

size_t splitComm(char *buffer, char **comm) {
	size_t c = 0;
	char *token = strtok(buffer, "|");
	while (token != NULL && c < CommSize) {
		comm[c++] = token;
		token = strtok(NULL, "|");
	}
	comm[c] = NULL;

	return c;
}

void splitArg(char *buffer, char **args) {
	size_t i = 0;
	char *token = strtok(buffer, " ");
	while (token != NULL && i < CommSize) {
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
}