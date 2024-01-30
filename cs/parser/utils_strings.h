#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Makes the right malloc call and copy semantics for C
// strings
char *Utils_Strings_Copy(const char *input, char **output);

bool Utils_Strings_HasPrefix(const char *thisString,
                             const char *prefix);

#endif  // STRING_UTILS_H