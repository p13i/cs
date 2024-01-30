#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "utils_strings.h"

// Makes the right malloc call and copy semantics for C strings
char *Utils_Strings_Copy(const char *input, char **output)
{
	size_t len = strlen(input);
	char *copy = (char *)malloc(sizeof(char) * (len + 1));
	strcpy(copy, input);
	copy[len] = '\0';
	*output = copy;
}

bool Utils_Strings_HasPrefix(const char *thisString, const char *prefix)
{
	int thisStringCursor = 0;
	int prefixCursor = 0;

	while ('\0' != prefix[prefixCursor]) {
		if (thisString[thisStringCursor] == '\0' || 
			thisString[thisStringCursor] != prefix[prefixCursor])
		{
			return false;
		}
		thisStringCursor++;
		prefixCursor++;
	}

	return true;
}
