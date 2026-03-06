CC = gcc
CFLAGS = -O2 -g -std=c11 -pedantic -Wall -Wextra
LD_LIBS = -lm
OBJ_DIR = obj
BIN_DIR = bin
ERROR_OBJ = $(OBJ_DIR)/error.o
ERAT_OBJ = $(OBJ_DIR)/eratosthenes.o
ERAT_OBJ_I = $(OBJ_DIR)/eratosthenes-i.o
PRIMES_OBJ = $(OBJ_DIR)/primes.o
PRIMES_OBJ_I = $(OBJ_DIR)/primes-i.o

PRIMES_BIN = $(BIN_DIR)/primes

.PHONY: all run clean

all: $(PRIMES_BIN) $(PRIMES_BIN)-i

$(PRIMES_BIN): $(ERROR_OBJ) $(ERAT_OBJ) $(PRIMES_OBJ) | $(BIN_DIR)
	$(CC) $(ERROR_OBJ) $(ERAT_OBJ) $(PRIMES_OBJ) $(LD_LIBS) -o $@

$(PRIMES_BIN)-i: $(ERROR_OBJ) $(ERAT_OBJ_I) $(PRIMES_OBJ_I) | $(BIN_DIR)
	$(CC) $(ERROR_OBJ) $(ERAT_OBJ_I) $(PRIMES_OBJ_I) $(LD_LIBS) -o $@

$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(ERROR_OBJ): error.c error.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c error.c -o $@

$(ERAT_OBJ): eratosthenes.c bitarray.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c eratosthenes.c -o $(ERAT_OBJ)

$(ERAT_OBJ_I): eratosthenes.c bitarray.h | $(OBJ_DIR)
	$(CC) -DUSE_INLINE $(CFLAGS) -c eratosthenes.c -o $(ERAT_OBJ_I)

$(PRIMES_OBJ): primes.c bitarray.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c primes.c -o $(PRIMES_OBJ)

$(PRIMES_OBJ_I): primes.c bitarray.h | $(OBJ_DIR)
	$(CC) -DUSE_INLINE $(CFLAGS) -c primes.c -o $(PRIMES_OBJ_I)

run:
	ulimit -s 60000 && ./$(BIN_DIR)/primes
	ulimit -s 60000 && ./$(BIN_DIR)/primes-i
	
clean: 
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)