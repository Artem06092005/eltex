CC = gcc
CFLAGS = -Wall -g -I./src

SRC_DIR = src
BIN_DIR = bin
LIB_DIR= lib

SOURCES = $(SRC_DIR)/main.c  $(SRC_DIR)/menu.c

$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(LIB_DIR))

res: $(SRC_DIR)/main.c $(SRC_DIR)/menu.c $(LIB_DIR)/libcontactList.a
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/menu.c -o $(BIN_DIR)/res -L./$(LIB_DIR) -lcontactList


$(LIB_DIR)/libcontactList.a: contactList.o
	ar rc $(LIB_DIR)/libcontactList.a $(LIB_DIR)/contactList.o


contactList.o: $(SRC_DIR)/contactList.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/contactList.c -o $(LIB_DIR)/contactList.o

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(LIB_DIR)

.PHONY: run
run: $(BIN_DIR)/res
	$(BIN_DIR)/res