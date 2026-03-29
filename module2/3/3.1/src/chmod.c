#include "chmod.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

uint16_t getBitMask(char *str) {
	uint16_t mask = 0;
	int len = strlen(str);

	if (str[0] >= '0' && str[0] <= '7') {
		if (len != 3) return 0xFFFF;
		for (int i = 0; i < 3; i++) {
			if (str[i] < '0' || str[i] > '7') return 0xFFFF;
		}
		unsigned int octal;
		sscanf(str, "%o", &octal);
		return (uint16_t)octal;
	}

	if (len != 9) return 0xFFFF;

	for (int i = 0; i < 9; i++) {
		if (str[i] == 'r')
			mask |= (1 << (8 - i));
		else if (str[i] == 'w')
			mask |= (1 << (8 - i));
		else if (str[i] == 'x')
			mask |= (1 << (8 - i));
		else if (str[i] != '-')
			return 0xFFFF;
	}
	return mask;
}

int getStatString(char *filename, char *dest) {
	struct stat fileStat;
	if (stat(filename, &fileStat) == -1) return -1;

	uint16_t mode = fileStat.st_mode & 0777;
	char rwx[10] = "---------";
	char comps[] = "rwxrwxrwx";

	for (int i = 0; i < 9; i++) {
		if (mode & (1 << (8 - i))) rwx[i] = comps[i];
	}

	if (dest) strcpy(dest, rwx);
	return (int)mode;
}

void printBinary(uint16_t mask) {
	printf("0b");
	for (int i = 8; i >= 0; i--) {
		printf("%d", (mask >> i) & 1);
		if (i % 3 == 0 && i != 0) printf("_");
	}
}
static uint16_t parse_symbolic_part(uint16_t current, const char *part) {
	uint16_t who_mask = 0;
	int i = 0;

	while (part[i] == 'u' || part[i] == 'g' || part[i] == 'o' ||
		   part[i] == 'a') {
		if (part[i] == 'u')
			who_mask |= 0700;
		else if (part[i] == 'g')
			who_mask |= 0070;
		else if (part[i] == 'o')
			who_mask |= 0007;
		else if (part[i] == 'a')
			who_mask |= 0777;
		i++;
	}

	if (who_mask == 0) who_mask = 0777;

	if (part[i] != '+' && part[i] != '-' && part[i] != '=') return 0xFFFF;
	char op = part[i++];

	uint16_t perm_bits = 0;
	while (part[i]) {
		if (part[i] == 'r')
			perm_bits |= (0444 & who_mask);
		else if (part[i] == 'w')
			perm_bits |= (0222 & who_mask);
		else if (part[i] == 'x')
			perm_bits |= (0111 & who_mask);
		else
			return 0xFFFF;
		i++;
	}

	if (op == '+') return current | perm_bits;
	if (op == '-') return current & ~perm_bits;
	if (op == '=') return (current & ~who_mask) | perm_bits;

	return 0xFFFF;
}

uint16_t simulateChmod(char *mode_str, char *filename) {
	struct stat fileStat;
	if (stat(filename, &fileStat) == -1) return 0xFFFF;

	uint16_t currentMode = fileStat.st_mode & 0777;

	uint16_t result = getBitMask(mode_str);
	if (result != 0xFFFF) return result;

	char temp[256];
	strncpy(temp, mode_str, sizeof(temp));
	char *token = strtok(temp, ",");
	result = currentMode;

	while (token != NULL) {
		result = parse_symbolic_part(result, token);
		if (result == 0xFFFF) return 0xFFFF;
		token = strtok(NULL, ",");
	}

	return result;
}