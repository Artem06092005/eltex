CC=gcc

all: main
	

main: main.c LinkedList.c driverRun.c dispatcher.c
	$(CC) -o main main.c LinkedList.c driverRun.c dispatcher.c


.PHONY: clean
clean:
	rm -f main 