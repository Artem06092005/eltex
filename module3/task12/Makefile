CC=gcc

all: client server

client: client.c
	$(CC) client.c -o client

server: server.c 
	$(CC) server.c -o server

.PHONY: clear
clear:
	rm -rf client server