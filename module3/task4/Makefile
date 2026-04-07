CC=gcc
CFLAGS=-Wall -Wextra 
SRC_DIR=src
BIN_DIR=bin


$(shell mkdir -p $(BIN_DIR))

all: $(BIN_DIR)/res $(BIN_DIR)/sum $(BIN_DIR)/maxlen

$(BIN_DIR)/res: $(SRC_DIR)/main.c $(SRC_DIR)/execute_pipeline.c $(SRC_DIR)/parser.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/sum: $(SRC_DIR)/sum.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/maxlen: $(SRC_DIR)/maxlen.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: run
run: $(BIN_DIR)/res
	./$(BIN_DIR)/res

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*