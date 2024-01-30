#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "parser.h"
#include "runtime.h"
#include "debug.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "parser.h"
#include "debug.h"
#include "runtime.h"
#include "utils_strings.h"
#include "parser_tests.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Expected 1 arg but got %d\n", argc - 1),
			printf("Usage: ./parser \"add(1, 2)\"\n");
		return -1;
	}

	if (0 == strcmp(argv[1], "tests"))
	{
		return Parser_Tests_main(argc, argv);
	}

	char *string;
	if ('.' == argv[1][0])
	{
		char *filename = argv[1];
		string = readFileContentsToString(filename);
	}
	else
	{
		string = argv[1];
	}

	printf("Evaluating:\n%s\n", string);

	int result;
	int cursor = 0;
	bool success = Parser_TryParseExpression(string, &cursor, &result);

	if (true == success)
	{
		printf("Result: %d\n", result);
		return 0;
	}
	else
	{
		Parser_ShowError(string, cursor);
		printf("Error!\n");
		return -1;
	}
}

// Source: https://stackoverflow.com/a/174552
static char *readFileContentsToString(char *filename)
{
	FILE *f = fopen(filename, "rb");

	if (NULL == f)
	{
		printf("Unable to load file: %s", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *buffer = (char *)malloc(length);
	if (NULL == buffer)
	{
		return NULL;
	}

	fread(buffer, 1, length, f);
	fclose(f);

	return buffer;
}

bool Parser_TryEvaluateFile(char *filename, int *result)
{
	char *string = readFileContentsToString(filename);
	return Parser_TryEvaluate(string, result);
}

bool Parser_TryEvaluate(char *string, int *result)
{
	Runtime_Init();
	int cursor = 0;
	return Parser_TryParseExpression(string, &cursor, result);
}

bool Parser_IsValidCharForVarName(const char charInVar)
{
	return ('A' <= charInVar && charInVar <= 'Z') || (charInVar == '_');
}

bool Parser_TryGetVariableName(char *string, int *cursor, char **varName)
{
	int start = *cursor;

	while (true == Parser_IsValidCharForVarName(string[*cursor]))
	{
		*cursor += 1;
	}

	int varNameLength = *cursor - start;
	// printf("Got var name length as %d\n", varNameLength);

	if (varNameLength == 0)
	{
		ERROR("Variable name must start with A-Z or _, got: %c\n", string[*cursor]);
		SHOW_CONTEXT();
		return false;
	}

	// printf("Got var name length as %d\n", varNameLength);

	// printf("Making copy of name...\n");

	char *name = (char *)malloc(sizeof(char) * (varNameLength + 1));
	strncpy(name, string + start, varNameLength);
	name[varNameLength] = '\0';
	*varName = name;

	// printf("Copied name to '%s'\n", name);

	// *cursor += varNameLength;

	return true;
}

#if 0
bool Parser_TryForwardUntilEndOfOp(char *string, int *cursor, int *result)
{
	// By the end of the string, the value of parenCount should be == -1
	// because the op's `xxx(` was consumed before this function was called
	int parenCount = 0;
	while ('\0' != string[*cursor] && parenCount != -1)
	{
		if ('(' == string[*cursor])
		{
			parenCount += 1;
		}
		else if (')' == string[*cursor])
		{
			parenCount -= 1;
		}
		*cursor += 1;
	}

	// Need to consume one extra closing paren ')'
	EXPECT_TRUE_OR_ERROR(
		parenCount != -1,
		"Reached end of string and did not find a closing paren.\n");

	return true;
}
#endif

bool Parser_TryParseExpression(char *string, int *cursor, int *result)
{
	DEBUG("Parser_TryParseExpression(...) -> %s\n", string + *cursor);
	if (((char *)NULL) == string || '\0' == string[*cursor])
	{
		ERROR("Null or empty string");
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	if ('0' <= string[*cursor] && string[*cursor] <= '9' ||
		'-' == string[*cursor] || '+' == string[*cursor])
	{
		return Parser_TryGetInt(string, cursor, result);
	}
	else if ('a' <= string[*cursor] && string[*cursor] <= 'z')
	{
		Parser_OpType_t opCode;
		bool success = Parser_TryGetOpCode(string, cursor, &opCode);
		if (false == success)
		{
			ERROR("Couldn't get op code!\n");
			return false;
		}

		DEBUG("!!! Got opCode '%c'\n", opCode);

		success = Parser_TryApplyOpCode(string, cursor, opCode, result);
		if (false == success)
		{
			ERROR("Unable to apply op code '%c'\n", opCode);
			return false;
		}

		return true;
	}

	ERROR("Failed to parse expression due to unknown or unexpected char '%c'\n", string[*cursor]);
	SHOW_CONTEXT();
	return false;
}

// Parses positive ints with no sign
bool Parser_TryGetInt(char *string, int *cursor, int *result)
{
	int sign = +1;
	if ('-' == string[*cursor])
	{
		sign = -1;
		*cursor += 1;
	}
	else if ('+' == string[*cursor])
	{
		sign = +1;
		*cursor += 1;
	}

	// First count num chars in string
	int length = 0;
	while ('0' <= string[*cursor + length] && string[*cursor + length] <= '9')
	{
		length++;
	}

	if (0 == length)
	{
		ERROR("No int found!\n");
		return false;
	}
	// printf("Length of int at '%s' is %d\n", string, length);

	// Now evaluate the int from the back
	int value = 0;
	int power = 1;
	int i = *cursor + length - 1;
	while (i >= *cursor)
	{
		// printf("%d >= %d\n", i, *cursor);
		value += (string[i] - '0') * power;
		// printf("value += (string[i] - '0') * power;\n");
		// printf("value += (%c - '0') * %d;\n", string[i], power);

		power *= 10;
		i--;
	}

	*result = sign * value;
	*cursor += length;
	return true;
}

void Parser_ShowError(const char *string, const int cursor)
{
	printf("\nSee:\n");
	const int maxCharsForContext = 16;
	int i = MAX(cursor - (maxCharsForContext / 2), 0);
	int count = 0;
	while (string[i] && count <= maxCharsForContext)
	{
		printf("%c", string[i]);
		i++;
		count++;
	}
	printf("\n");

	i = MAX(cursor - (maxCharsForContext / 2), 0);
	while (string[i] && i < cursor)
	{
		printf(" ");
		i++;
	}
	printf("^\n\n");
}

void Parser_MaybeConsumeWhitespace(char *string, int *cursor)
{
	// Consume space, newline, and tab characters
	while (true)
	{
		if (string[*cursor] == '#')
		{
			*cursor += 1;
			while (string[*cursor] != '\n')
			{
				*cursor++;
			}
		}
		else if (string[*cursor] == ' ' || string[*cursor] == '\n' || string[*cursor] == '\t')
		{
			*cursor += 1;
		}
		else
		{
			break;
		}
	}
}

bool Parser_TryConsumeThisChar(char *string, int *cursor, char thisChar)
{
	if (thisChar != string[*cursor])
	{
		ERROR("Expected '%c' at index %d, but got '%c'\n", thisChar, *cursor, string[*cursor]);
		SHOW_CONTEXT();
		return false;
	}

	*cursor += 1;
	return true;
}
#include "parser_expectations.h"

bool Parser_TryGetOpCode(char *string, int *cursor, Parser_OpType_t *opCode)
{
	if (Utils_Strings_HasPrefix(string + *cursor, "add("))
	{
		*cursor += 4;
		*opCode = OP_ADD;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "sub("))
	{
		*cursor += 4;
		*opCode = OP_SUB;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "mul("))
	{
		*cursor += 4;
		*opCode = OP_MUL;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "div("))
	{
		*cursor += 4;
		*opCode = OP_DIV;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "neg("))
	{
		*cursor += 4;
		*opCode = OP_NEG;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "exe("))
	{
		*cursor += 4;
		*opCode = OP_EXE;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "var("))
	{
		*cursor += 4;
		*opCode = OP_VAR;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "get("))
	{
		*cursor += 4;
		*opCode = OP_GET;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "set("))
	{
		*cursor += 4;
		*opCode = OP_SET;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "iff("))
	{
		*cursor += 4;
		*opCode = OP_IFF;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "ife("))
	{
		*cursor += 4;
		*opCode = OP_IFE;
		return true;
	}

	if (Utils_Strings_HasPrefix(string + *cursor, "equ("))
	{
		*cursor += 4;
		*opCode = OP_EQU;
		return true;
	}

	ERROR("Unknown op code at index %d.\n", *cursor);
	SHOW_CONTEXT();

	return false;
}

bool Parser_TryApplyOp_get(char *string, int *cursor, int *result)
{
	DEBUG("Parser_TryApplyOp_get(%s,%d,...)\n", string, *cursor);

	Parser_MaybeConsumeWhitespace(string, cursor);

	char *varName;
	bool success = Parser_TryGetVariableName(string, cursor, &varName);
	if (false == success)
	{
		ERROR("Failed to get variable name %c.\n", varName);
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	int varValue;
	success = Runtime_TryGetVarValue(varName, &varValue);
	if (false == success)
	{
		ERROR("Failed to get var %c.\n", varName);
		return false;
	}

	DEBUG("!!! varValue = %d\n", varValue);

	Parser_MaybeConsumeWhitespace(string, cursor);

	if (false == Parser_TryConsumeThisChar(string, cursor, ')'))
	{
		ERROR("Didn't find ')'\n");
		return false;
	}

	*result = varValue;
	return true;
}

bool Parser_TryApplyOp_set(char *string, int *cursor, int *result)
{
	Parser_MaybeConsumeWhitespace(string, cursor);

	char *varName;
	bool success = Parser_TryGetVariableName(string, cursor, &varName);
	if (false == success)
	{
		ERROR("Failed to get variable name %c.\n", varName);
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	if (false == Parser_TryConsumeThisChar(string, cursor, ','))
	{
		ERROR("Unable to eat ,\n");
		SHOW_CONTEXT();
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	// Find what value to set this variable to
	int varValue;
	success = Parser_TryParseExpression(string, cursor, &varValue);
	if (false == success)
	{
		ERROR("Failed to get var '%c'.\n", varName);
		return false;
	}

	// Now actually set it
	success = Runtime_TrySetVarToValue(varName, varValue);
	if (false == success)
	{
		ERROR("Failed to set variable '%s' to value %d!\n", varName, varValue);
		return false;
	}

	*result = 0;

	Parser_MaybeConsumeWhitespace(string, cursor);
	success = Parser_TryConsumeThisChar(string, cursor, ')');
	if (false == success)
	{
		ERROR("Could not find ')', found '%c' instead!\n", string[*cursor])
		SHOW_CONTEXT();
		return false;
	}

	return true;
}

bool Parser_TryApplyOp_var(char *string, int *cursor, int *result)
{
	Parser_MaybeConsumeWhitespace(string, cursor);

	char *nameResult;
	bool success = Parser_TryGetVariableName(string, cursor, &nameResult);
	if (false == success)
	{
		ERROR("Unable to find variable name!\n");
		SHOW_CONTEXT();
		return false;
	}

	success = Runtime_TryAllocVar(nameResult);
	if (false == success)
	{
		ERROR("Unable to allocate variable name '%s'!\n", nameResult);
		SHOW_CONTEXT();
		return false;
	}

	// *cursor += strlen(nameResult);

	*result = 0;

	Parser_MaybeConsumeWhitespace(string, cursor);
	success = Parser_TryConsumeThisChar(string, cursor, ')');
	if (false == success)
	{
		ERROR("Could not find ')', found '%c' instead!\n", string[*cursor])
		SHOW_CONTEXT();
		return false;
	}

	return true;
}

bool Parser_TryApplyOp_exe(char *string, int *cursor, int *result)
{
	DEBUG("Parser_TryApplyOp_exe(string=%s, *cursor=%d, _)\n", string, *cursor);
	bool success;
	while (string[*cursor])
	{
		Parser_MaybeConsumeWhitespace(string, cursor);
		DEBUG("!!! Next substring: %s\n", string + *cursor);

		if (')' == string[*cursor])
		{
			DEBUG("Found closing paren ) here:\n");
			SHOW_CONTEXT();
			break;
		}

		success = Parser_TryParseExpression(string, cursor, result);
		if (false == success)
		{
			ERROR("Unable to parse expression!\n");
			return false;
		}

		Parser_MaybeConsumeWhitespace(string, cursor);

		if (')' == string[*cursor])
		{
			DEBUG("Found closing paren ) here:\n");
			SHOW_CONTEXT();
			break;
		}

		success = Parser_TryConsumeThisChar(string, cursor, ',');
		if (false == success)
		{
			ERROR("Unable to consume ','\n");
			SHOW_CONTEXT();
			return false;
		}

		Parser_MaybeConsumeWhitespace(string, cursor);
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	success = Parser_TryConsumeThisChar(string, cursor, ')');
	if (false == success)
	{
		ERROR("Unable to consume )\n");
		return false;
	}

	DEBUG("Done with Parser_TryApplyOp_exe\n");

	return true;
}

bool Parser_TryApplyOp_BasicFourBinaryArithmeticOps(char *string, int *cursor, int *result, Parser_OpType_t opCode)
{
	Parser_MaybeConsumeWhitespace(string, cursor);

	int firstArgValue;
	bool success = Parser_TryParseExpression(string, cursor, &firstArgValue);
	if (false == success)
	{
		ERROR("Unable to evaluate expression!\n");
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);
	success = Parser_TryConsumeThisChar(string, cursor, ',');
	if (false == success)
	{
		ERROR("Couldn't consume comma!\n");
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	int secondArgValue;
	success = Parser_TryParseExpression(string, cursor, &secondArgValue);
	if (false == success)
	{
		ERROR("Couldn't evalute expression!\n");
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	if (OP_ADD == opCode)
	{
		*result = firstArgValue + secondArgValue;
	}
	else if (OP_SUB == opCode)
	{
		*result = firstArgValue - secondArgValue;
	}
	else if (OP_MUL == opCode)
	{
		*result = firstArgValue * secondArgValue;
	}
	else if (OP_DIV == opCode)
	{
		*result = firstArgValue / secondArgValue;
	}
	else
	{
		ERROR("Unknown op code '%c'!\n", opCode);
		return false;
	}

	success = Parser_TryConsumeThisChar(string, cursor, ')');
	if (false == success)
	{
		ERROR("unable to consume closing paranthesis\n");
		SHOW_CONTEXT();
		return false;
	}
}

bool Parser_TryApplyOp_Neg(char *string, int *cursor, int *result)
{
	Parser_MaybeConsumeWhitespace(string, cursor);

	int firstArgValue;
	bool success = Parser_TryParseExpression(string, cursor, &firstArgValue);
	if (false == success)
	{
		ERROR("Unable to parse expression!\n");
		return false;
	}

	Parser_MaybeConsumeWhitespace(string, cursor);

	*result = -1 * firstArgValue;

	return Parser_TryConsumeThisChar(string, cursor, ')');
}

// Simple if.
// Syntax: ` iff( 1|0 , exe( ) ) `
// We start here ^
bool Parser_TryApplyOp_iff(char *string, int *cursor, int *result)
{
	DEBUG("Parser_TryApplyOp_iff(string=%s, *cursor=%d, _)\n", string, *cursor);

	Parser_MaybeConsumeWhitespace(string, cursor);

	int condResult;
	EXPECT_TRUE_OR_ERROR(
		Parser_TryParseExpression(string, cursor, &condResult),
		"Unable to parser condition argument for iff op.\n");

	// If we have 1, then run the next function, if we have 0, forward to end of
	// string because 0 is false
	if (condResult == 1)
	{
		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ','),
			"Unable to consume comma\n");

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryParseExpression(string, cursor, result),
			"Unable to parse expression in iff true cond block.\n");
		;

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ')'),
			"Unable to consume closing paren\n");

		return true;
	}
	else
	{
		// scan to end of this op
		int parenCount = 0;
		while ('\0' != string[*cursor] && parenCount >= 0)
		{
			DEBUG("Scrolling to end of string, evaluating substring: %s\n", string + *cursor);
			if ('(' == string[*cursor])
			{
				parenCount += 1;
			}
			else if (')' == string[*cursor])
			{
				parenCount -= 1;
			}
			DEBUG("Updated parenCount = %d\n", parenCount);
			*cursor += 1;
		}

		// We need to find one extra closing paren ')' since the opcode checking
		// consumed the opening ')'
		EXPECT_TRUE_OR_ERROR(
			-1 == parenCount,
			"No ending paren found for op.\n");

		return true;
	}
}

// An if-then-else three-argument operation
// Syntax: `ife( 1|0 , exe( ) , exe( ) )
bool Parser_TryApplyOp_ife(char *string, int *cursor, int *result)
{
	DEBUG("Parser_TryApplyOp_iff(string=%s, *cursor=%d, _)\n", string, *cursor);

	Parser_MaybeConsumeWhitespace(string, cursor);

	int condResult;
	EXPECT_TRUE_OR_ERROR(
		Parser_TryParseExpression(string, cursor, &condResult),
		"Unable to parser condition argument for iff op.\n");

	// If we have 1, then run the next function
	if (condResult == 1)
	{
		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ','),
			"Unable to consume comma\n");

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryParseExpression(string, cursor, result),
			"Unable to parse expression in iff true cond block.\n");
		;

		Parser_MaybeConsumeWhitespace(string, cursor);

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ','),
			"Unable to consume comma\n");

		// Scan to first opening paren
		while ('(' != string[*cursor])
		{
			*cursor += 1;
		}

		// scan to end of this op
		int parenCount = 0;
		while ('\0' != string[*cursor])
		{
			DEBUG("Scrolling to end of string, evaluating substring: %s\n", string + *cursor);
			if ('(' == string[*cursor])
			{
				parenCount += 1;
			}
			else if (')' == string[*cursor])
			{
				parenCount -= 1;
			}
			DEBUG("Updated parenCount = %d\n", parenCount);
			*cursor += 1;

			if (-1 == parenCount)
			{
				DEBUG("Found closing paren to end skipped ife true block");
				break;
			}
		}

		// We need to find one extra closing paren ')' since the opcode checking
		// consumed the opening ')'
		EXPECT_TRUE_OR_ERROR(
			-1 == parenCount,
			"No ending paren found for op.\n");

		Parser_MaybeConsumeWhitespace(string, cursor);

		return true;
	}
	// If we have 0, forward to end of the first expression because 0 is false
	else
	{
		// Scan to first opening paren
		while ('(' != string[*cursor])
		{
			*cursor += 1;
		}

		// scan to end of this op
		int parenCount = 0;
		while ('\0' != string[*cursor])
		{
			DEBUG("Scrolling to end of string, evaluating substring: %s\n", string + *cursor);
			if ('(' == string[*cursor])
			{
				parenCount += 1;
			}
			else if (')' == string[*cursor])
			{
				parenCount -= 1;
			}
			DEBUG("Updated parenCount = %d\n", parenCount);
			*cursor += 1;

			if (0 == parenCount)
			{
				DEBUG("Found closing paren to end skipped ife true block");
				break;
			}
		}

		EXPECT_TRUE_OR_ERROR(
			0 == parenCount,
			"No ending paren found for op.\n");

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ','),
			"Unable to consume arg separator ',' to evaluate else block\n");

		Parser_MaybeConsumeWhitespace(string, cursor);
		// Now apply the logic
		EXPECT_TRUE_OR_ERROR(
			Parser_TryParseExpression(string, cursor, result),
			"Unable to parse else-block expression\n");

		Parser_MaybeConsumeWhitespace(string, cursor);

		EXPECT_TRUE_OR_ERROR(
			Parser_TryConsumeThisChar(string, cursor, ')'),
			"Unable to consume closing paren\n");

		return true;
	}
}

bool Parser_TryApplyOp_equ(char *string, int *cursor, int *result)
{
	Parser_MaybeConsumeWhitespace(string, cursor);

	int firstArgValue;
	EXPECT_TRUE_OR_ERROR(
		Parser_TryParseExpression(string, cursor, &firstArgValue),
		"Unable to evalute first argument of equals op");

	Parser_MaybeConsumeWhitespace(string, cursor);

	EXPECT_TRUE_OR_ERROR(
		Parser_TryConsumeThisChar(string, cursor, ','),
		"Unable to consume arg separator ',' to between equals args\n");

	Parser_MaybeConsumeWhitespace(string, cursor);

	int secondArgValue;
	EXPECT_TRUE_OR_ERROR(
		Parser_TryParseExpression(string, cursor, &secondArgValue),
		"Unable to evalute second argument of equals op");

	Parser_MaybeConsumeWhitespace(string, cursor);

	EXPECT_TRUE_OR_ERROR(
		Parser_TryConsumeThisChar(string, cursor, ')'),
		"Unable to consume closing paren\n");

	if (firstArgValue == secondArgValue)
	{
		*result = 1;
	}
	else
	{
		*result = 0;
	}
	return true;
}

bool Parser_TryApplyOpCode(char *string, int *cursor, Parser_OpType_t opCode, int *result)
{
	if (OP_EXE == opCode)
	{
		return Parser_TryApplyOp_exe(string, cursor, result);
	}
	else if (OP_GET == opCode)
	{
		return Parser_TryApplyOp_get(string, cursor, result);
	}
	else if (OP_SET == opCode)
	{
		return Parser_TryApplyOp_set(string, cursor, result);
	}
	else if (OP_VAR == opCode)
	{
		return Parser_TryApplyOp_var(string, cursor, result);
	}
	else if (OP_ADD == opCode ||
			 OP_SUB == opCode ||
			 OP_MUL == opCode ||
			 OP_DIV == opCode)
	{
		return Parser_TryApplyOp_BasicFourBinaryArithmeticOps(string, cursor, result, opCode);
	}
	else if (OP_NEG == opCode)
	{
		return Parser_TryApplyOp_Neg(string, cursor, result);
	}
	else if (OP_IFF == opCode)
	{
		return Parser_TryApplyOp_iff(string, cursor, result);
	}
	else if (OP_IFE == opCode)
	{
		return Parser_TryApplyOp_ife(string, cursor, result);
	}
	else if (OP_EQU == opCode)
	{
		return Parser_TryApplyOp_equ(string, cursor, result);
	}

	ERROR("Unknown op code '%c'\n", opCode);
	return false;
}
