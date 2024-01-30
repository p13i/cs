#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

// Makes the right malloc call and copy semantics for C strings
char *Utils_Strings_Copy(const char *input, char **output);

bool Utils_Strings_HasPrefix(const char *thisString, const char *prefix);

#endif // STRING_UTILS_H