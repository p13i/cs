#include "parser.h"
#include "debug.h"

bool Parser_TryConsumeThiString(char *string, int *cursor, const char *thisString)
{
    int thisCursor = 0;
    while ('\0' != thisString[thisCursor]) {
        if (string[*cursor] != thisString[thisCursor]) {
            ERROR("Unable to consume entire target string '%s' with given string.\n");
            SHOW_CONTEXT();
            return false;
        }

        thisCursor++;
        *cursor += 1;
    }

    return true;
}
