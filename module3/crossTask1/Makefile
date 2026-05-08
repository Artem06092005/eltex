CC=gcc

all: client server
	

client: client.c 
	$(CC) -o client client.c headerUDP.c

server: server.c 
	$(CC) -o server server.c LinkedList.c headerUDP.c

.PHONY: clean
clean:
	rm -f client server