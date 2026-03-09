# COMPILER A FLAGY #
CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
LD_LIBS = -lm

# SLOZKY #
OBJ_DIR = obj
BIN_DIR = bin

# UMISTENI OBJEKTU #
ERROR_OBJ = $(OBJ_DIR)/error.o
ERAT_OBJ = $(OBJ_DIR)/eratosthenes.o
ERAT_OBJ_I = $(OBJ_DIR)/eratosthenes-i.o
PRIMES_OBJ = $(OBJ_DIR)/primes.o
PRIMES_OBJ_I = $(OBJ_DIR)/primes-i.o
NC_OBJ = $(OBJ_DIR)/no-comment.o

# UMISTENI BINAREK #
NC_BIN = $(BIN_DIR)/no-comment
PRIMES_BIN = $(BIN_DIR)/primes
PRIMES_BIN_I = $(BIN_DIR)/primes-i


.PHONY: all run clean

all: $(PRIMES_BIN) $(PRIMES_BIN_I) $(NC_BIN)

run: $(PRIMES_BIN) $(PRIMES_BIN_I)
	ulimit -s 60000 && ./$(BIN_DIR)/primes
	ulimit -s 60000 && ./$(BIN_DIR)/primes-i

# SPUSTITELNE BINARKY #
$(PRIMES_BIN): $(ERROR_OBJ) $(ERAT_OBJ) $(PRIMES_OBJ) | $(BIN_DIR)
	$(CC) $(ERROR_OBJ) $(ERAT_OBJ) $(PRIMES_OBJ) $(LD_LIBS) -o $@

$(PRIMES_BIN_I): $(ERROR_OBJ) $(ERAT_OBJ_I) $(PRIMES_OBJ_I) | $(BIN_DIR)
	$(CC) $(ERROR_OBJ) $(ERAT_OBJ_I) $(PRIMES_OBJ_I) $(LD_LIBS) -o $@

$(NC_BIN): $(NC_OBJ) $(ERROR_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(ERROR_OBJ) $(NC_OBJ) -o $@

# OBJEKTY #
$(ERROR_OBJ): error.c error.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c error.c -o $@

$(NC_OBJ): no-comment.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c no-comment.c -o $@

$(ERAT_OBJ): eratosthenes.c bitarray.h | $(OBJ_DIR)
	$(CC) -O2 $(CFLAGS) -c eratosthenes.c -o $(ERAT_OBJ)

$(ERAT_OBJ_I): eratosthenes.c bitarray.h | $(OBJ_DIR)
	$(CC) -O2 -DUSE_INLINE $(CFLAGS) -c eratosthenes.c -o $(ERAT_OBJ_I)

$(PRIMES_OBJ): primes.c bitarray.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c primes.c -o $(PRIMES_OBJ)

$(PRIMES_OBJ_I): primes.c bitarray.h | $(OBJ_DIR)
	$(CC) -DUSE_INLINE $(CFLAGS) -c primes.c -o $(PRIMES_OBJ_I)

# PRIPADNE VYTVORENI SLOZEK #
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	
clean: 
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)