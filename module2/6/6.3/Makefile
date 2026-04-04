CC = gcc
CFLAGS = -Wall -g -fPIC
SRC_DIR = src
LIB_DIR = lib

$(shell mkdir -p $(LIB_DIR))

all: $(LIB_DIR)/sum_two.so $(LIB_DIR)/sub.so $(LIB_DIR)/mul.so $(LIB_DIR)/div.so res

$(LIB_DIR)/sum_two.so: $(SRC_DIR)/sum_two.c
	$(CC) $(CFLAGS) -shared -o $@ $<

$(LIB_DIR)/sub.so: $(SRC_DIR)/sub.c
	$(CC) $(CFLAGS) -shared -o $@ $<

$(LIB_DIR)/mul.so: $(SRC_DIR)/mul.c
	$(CC) $(CFLAGS) -shared -o $@ $<

$(LIB_DIR)/div.so: $(SRC_DIR)/div.c
	$(CC) $(CFLAGS) -shared -o $@ $< -lm

res: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $< -o $@ -ldl -lm


LIBS=$(LIB_DIR)/sum_two.so $(LIB_DIR)/sub.so $(LIB_DIR)/mul.so $(LIB_DIR)/div.so
.PHONY: libs
libs: $(LIBS)

.PHONY: clean
clean:
	rm -rf $(LIB_DIR) res

.PHONY: run
run: all
	LD_LIBRARY_PATH=./$(LIB_DIR) ./res