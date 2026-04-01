CC = gcc
CFLAGS = -Wall -g -I./src

SRC_DIR = src
BIN_DIR = bin
LIB_DIR= lib

SOURCES = $(SRC_DIR)/main.c  $(SRC_DIR)/menu.c

$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(LIB_DIR))

res: $(SRC_DIR)/main.c $(SRC_DIR)/menu.c
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/menu.c -o $(BIN_DIR)/res -L./$(LIB_DIR) -lcontactList


.PHONY: lib
lib:$(LIB_DIR)/libcontactList.so

$(LIB_DIR)/libcontactList.so: contactList.o
	$(CC) $(CFLAGS) -shared $(LIB_DIR)/contactList.o -o $(LIB_DIR)/libcontactList.so


contactList.o: $(SRC_DIR)/contactList.c
	$(CC) -fPIC $(CFLAGS) -c $(SRC_DIR)/contactList.c -o $(LIB_DIR)/contactList.o

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(LIB_DIR)

.PHONY: run
run: $(BIN_DIR)/res
	LD_LIBRARY_PATH=./$(LIB_DIR) ./bin/res
	