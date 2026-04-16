CC = gcc

all: prod cons

prod: producer.c
	$(CC) producer.c -o prod 

cons: consumer.c
	$(CC) consumer.c -o cons 

.PHONY: clear
clear:
	rm -rf prod cons