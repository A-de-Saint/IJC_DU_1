#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

void warning(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif