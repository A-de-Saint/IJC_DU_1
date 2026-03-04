CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
LD_LIBS = -lm
OBJ_DIR = obj
BIN = bin

.PHONY: all clean

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

error.o: error.c error.h
	${CC} ${CFLAGS} -c $< -o ${OBJ_DIR}/$@

clean: 
	rm -rf ${OBJ_DIR}