#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Аргументов слишком мало. Введите %s arg1 arg2 ...", argv[0]);
		return 0;
	}

	int max_len = strlen(argv[1]);
	char *longest = argv[1];

	for (int i = 2; i < argc; i++) {
		int len = strlen(argv[i]);
		if (len > max_len) {
			max_len = len;
			longest = argv[i];
		}
	}

	printf("Самая длинная строка: \"%s\" (длина %d)\n", longest, max_len);
	return 0;
}