#ifndef DEBUG_H
#define DEBUG_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR(fmt, ...)                           \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  printf(fmt, ##__VA_ARGS__);
#define SHOW_CONTEXT() Parser_ShowError(string, *cursor);

#define LOG_DEBUG false
#if LOG_DEBUG
#define DEBUG(fmt, ...)                  \
  printf("%s:%d: ", __FILE__, __LINE__); \
  printf(fmt, ##__VA_ARGS__);
#else
#define DEBUG(fmt, ...) ;  // print nothing
#endif                     // LOG_DEBUG

#endif  // DEBUG_H
