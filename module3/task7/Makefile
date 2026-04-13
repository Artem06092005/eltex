CC = gcc

all: prog1 prog2

prog1: prog1.c
	$(CC) prog1.c -o prog1 -lrt

prog2: prog2.c
	$(CC) prog2.c -o prog2 -lrt

.PHONY: clear
clear:
	rm -rf prog1 prog2