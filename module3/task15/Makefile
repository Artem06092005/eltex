CC=gcc

all: client server

client: client.o
	$(CC) -o client client.o

client.o: client.c
	$(CC) -c client.c

server: server.o stateHandler.o
	$(CC)  -o server server.o stateHandler.o

stateHandler.o: stateHandler.c stateHandler.h
	$(CC)  -c stateHandler.c

server.o: server.c stateHandler.h
	$(CC)  -c server.c

.PHONY: clear
clear:
	rm -rf client server