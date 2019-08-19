#ifndef _LOG_H_
#define _LOG_H_
#include <stdarg.h>
#include <stdio.h>

#define LOG(...) printf("Filename %s, Function %s, Line %d > ", __FILE__, __FUNCTION__, __LINE__); \
                            printf(__VA_ARGS__); \
                            printf("\n");
#endif