# Makefile
# Makefile pro IJC-DU1, 9.3.2026
# Autor: Antonín Hrnčíř, FIT

# COMPILER A FLAGY #
CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
LD_LIBS = -lm

# SLOZKY #
OBJ_DIR = obj
BIN_DIR = bin

# OBJEKTY #
NO_COMMENT_OBJS = $(OBJ_DIR)/no-comment.o $(OBJ_DIR)/error.o
PRIMES_OBJS = $(OBJ_DIR)/error.o $(OBJ_DIR)/eratosthenes.o $(OBJ_DIR)/primes.o
PRIMES_I_OBJS = $(OBJ_DIR)/error.o $(OBJ_DIR)/eratosthenes-i.o $(OBJ_DIR)/primes-i.o

# BINARKY #
NO_COMMENT_BIN = $(BIN_DIR)/no-comment
PRIMES_BIN = $(BIN_DIR)/primes
PRIMES_BIN_I = $(BIN_DIR)/primes-i

.PHONY: all run clean

all: $(PRIMES_BIN) $(PRIMES_BIN_I) $(NO_COMMENT_BIN)

run: $(PRIMES_BIN) $(PRIMES_BIN_I) $(NO_COMMENT_BIN)
	ulimit -s 60000 && ./$(BIN_DIR)/primes
	ulimit -s 60000 && ./$(BIN_DIR)/primes-i
	./$(BIN_DIR)/no-comment no-comment.c >no-comment.txt

# SPUSTITELNE BINARKY #
$(PRIMES_BIN): $(PRIMES_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LD_LIBS)

$(PRIMES_BIN_I): $(PRIMES_I_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LD_LIBS)

$(NO_COMMENT_BIN): $(NO_COMMENT_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@

# INLINE OBJEKTY #
$(OBJ_DIR)/%-i.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -DUSE_INLINE -c $< -o $@

# OBJEKTY #
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(OBJ_DIR)/*.d

# PRIPADNE VYTVORENI SLOZEK #
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

 $(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
clean: 
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)