#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

//prints formatted warning to stderr (finished with newline)
void warning(const char *fmt, ...);

//printfs formatted error to stderr (finished with newline) and exits program
void error_exit(const char *fmt, ...);

#endif