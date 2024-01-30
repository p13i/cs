#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "runtime.h"

#include "parser.h"
#include "debug.h"
#include "utils_strings.h"

#define RUNTIME_MAX_VARS 16

typedef struct
{
	char *name;
	int value;
} _VarAndValue_t;

static _VarAndValue_t *Runtime_VarsAndValues[RUNTIME_MAX_VARS];

void Runtime_Init()
{
	for (size_t i = 0; i < RUNTIME_MAX_VARS; i++)
	{
		free(Runtime_VarsAndValues[i]);
		Runtime_VarsAndValues[i] = NULL;
	}
}

static _VarAndValue_t *_GetVarAndValue(const char *varName)
{
	for (size_t i = 0; i < RUNTIME_MAX_VARS; i++)
	{
		if (NULL != Runtime_VarsAndValues[i])
		{
			char *thisName = Runtime_VarsAndValues[i]->name;
			if (0 == strcmp(varName, thisName))
			{
				return Runtime_VarsAndValues[i];
			}
		}
	}

#if 0
	ERROR("Couldn't find variable with name '%s' in Runtime.\n", varName);
	printf("Runtime dump:\n");

	for (size_t i = 0; i < RUNTIME_MAX_VARS; i++) {
		if (NULL == Runtime_VarsAndValues[i]) {
			printf("[%d] is NULL\n", i);
		}
		else {
			char* thisName = Runtime_VarsAndValues[i]->name;
			int thisValue = Runtime_VarsAndValues[i]->value;
			printf("[%d] is '%s' -> %d\n", i, thisName, thisValue);
		}
	}
#endif

	return NULL;
}

static size_t _GetEmptyIndexForNewVarAndValue()
{
	for (size_t i = 0; i < RUNTIME_MAX_VARS; i++)
	{
		if (NULL == Runtime_VarsAndValues[i])
		{
			return i;
		}
	}
	return -1;
}

static bool _IsValidVarName(const char *varName)
{
	size_t len = strlen(varName);
	if (0 == len)
	{
		return false;
	}

	for (size_t i = 0; i < len; i++)
	{
		char thisChar = varName[i];
		if (false == Parser_IsValidCharForVarName(thisChar))
		{
			ERROR("Variable name has invalid char '%c' at index %d\n", thisChar, i);
			Parser_ShowError(varName, i);
			return false;
		}
	}
	return true;
}

bool Runtime_IsVarAlloc(const char *varName)
{
	if (false == _IsValidVarName(varName))
	{
		ERROR("Invalid var name '%s'\n", varName);
		return false;
	}
	return NULL != _GetVarAndValue(varName);
}

bool Runtime_TryAllocVar(const char *varName)
{
	if (false == _IsValidVarName(varName))
	{
		ERROR("Invalid var name '%s'\n", varName);
		return false;
	}
	// If var is already present, error
	if (NULL != _GetVarAndValue(varName))
	{
		ERROR("Variable '%s' is already allocated!\n", varName);
		return false;
	}

	int emptyIndex = _GetEmptyIndexForNewVarAndValue();
	if (-1 == emptyIndex)
	{
		ERROR("Variable '%s' cannot be allocated because backing store is full!\n", varName);
		return false;
	}

	char* nameCopy;
	Utils_Strings_Copy(varName, &nameCopy);

	Runtime_VarsAndValues[emptyIndex] = (_VarAndValue_t *)malloc(sizeof(_VarAndValue_t));
	Runtime_VarsAndValues[emptyIndex]->name = nameCopy;
	Runtime_VarsAndValues[emptyIndex]->value = 0;

	return true;
}

bool Runtime_TryGetVarValue(const char *varName, int *result)
{
	_VarAndValue_t *varAndValue = _GetVarAndValue(varName);
	if (NULL == _GetVarAndValue(varName))
	{
		ERROR("Variable '%s' is not allocated!\n", varName);
		return false;
	}

	*result = varAndValue->value;
	return true;
}

bool Runtime_TrySetVarToValue(const char *varName, int value)
{
	DEBUG("Runtime_TrySetVarToValue(%s, %d)\n", varName, value);
	_VarAndValue_t *varAndValue = _GetVarAndValue(varName);
	if (NULL == _GetVarAndValue(varName))
	{
		ERROR("Variable '%s' is not allocated!\n", varName);
		return false;
	}

	varAndValue->value = value;
	return true;
}
