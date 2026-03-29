CC = gcc
CFLAGS = -Wall -g -I./src

SRC_DIR = src
BIN_DIR = bin
UNITY_DIR = unity
TEST_DIR= tests

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/contactList.c $(SRC_DIR)/menu.c
TARGET = $(BIN_DIR)/res
TEST_TARGET= $(BIN_DIR)/test
TEST_FILES=$(TEST_DIR)/testContactList.c $(UNITY_DIR)/unity.c $(SRC_DIR)/contactList.c


$(shell mkdir -p $(BIN_DIR))

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@


test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_FILES)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: run
run: $(TARGET)
	./$(TARGET)