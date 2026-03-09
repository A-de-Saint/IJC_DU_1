// error.c
// Řešení příkladu IJC-DU1, příklad b), 9.3.2026
// Autor: Antonín Hrnčíř, FIT
// Přeloženo: gcc 15.2.1
/* Implementace funkcí pro chybová hlášení z error.h */

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void warning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Warning: ");
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
}

void error_exit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
    exit(1);
}