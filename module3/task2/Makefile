CC=gcc

all: res sum maxlen

res: main.c 
	$(CC) main.c -o res

sum: sum.c 
	$(CC) sum.c -o sum

maxlen: maxlen.c 
	$(CC) maxlen.c -o maxlen

.PHONY: run
run: ./res
	./res

.PHONY: clean
clean:
	rm -rf maxlen sum res