CC = gcc
CFLAGS = -Wall -g -I./src

SRC_DIR = src
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/contactList.c $(SRC_DIR)/menu.c
TARGET = $(BIN_DIR)/res

all: $(TARGET)

$(TARGET): $(SOURCES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: run
run: $(TARGET)
	./$(TARGET)