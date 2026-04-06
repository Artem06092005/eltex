#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Аргументов слишком мало. Введите %s arg1 arg2 ...", argv[0]);
		return 0;
	}
	int res = 0;
	for (size_t i = 0; i < argc; i++) {
		res += atol(argv[i]);
	}
	printf("Сумма аргументов = %d\n", res);
}