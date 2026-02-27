CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
OBJ_DIR = obj
BIN = bin

mkdir:
	@mkdir -p ${OBJ_DIR}
	@mkdir -p ${BIN}

error: error.c
	${CC} ${CFLAGS} -c error.c -o ${OBJ_DIR}/error.o

clean: 
	rm -rf ${OBJ_DIR}