#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

////////////////////////////////////////////////////////////
//
// 	PARSER IN C
//
// 	A poorly-name interpreter for a new and fancy
//	programming language
//
// 	An expression can be a literal `int` number or an
// 	operation. Operations are pre-defined functions that
// 	apply logic to 0 or more arguments. The syntax of
// these 	operations is:
//
//      EXPRESSION  := LITERAL | OPERATION
//		OPERATION 	:=
//`CODE(EXPRESSION[,EXPRESSION...])`
//      LITERAL     := Regex `0-9]`
//      CODE        := Regex `[a-z]{3}`
//
// 	For example, all of these expressions are valid:
//
// 		`129847821`
// 		`add(23,49)`
// 		`add ( 23 , 59 ) `
//
//	Whitespace of any kind (tabs, newlines, or spaces)
//can 	be placed freely around the tokens of this simple
//	programming language interpreter, maybe call it
//	"p-lang"?
//
// 	And so on for the following:
//
// 		set, var, add, sub, mul, div, iff, ife, equ,
// neg,
//		exe,
//
//	and more on the way such as
//			for, fxn, ret, res,
//
//	There must be a comma `,` between arguments to an
//	operation. However, there cannot be a comma after
//the 	last argument of an operation. In imperitive
//operations 	like exe(...), the value of the last expression
//in this 	arguments list is the exe expression's "return
//value," 	so to speak.
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//	Main function
////////////////////////////////////////////////////////////

static char *readFileContentsToString(char *filename);
bool Parser_TryEvaluateFile(char *filename, int *result);

////////////////////////////////////////////////////////////
//	Supported operations
////////////////////////////////////////////////////////////

typedef enum {
  OP_ADD = '+',
  OP_SUB = '-',
  OP_DIV = '/',
  OP_MUL = '*',
  OP_NEG = 'n',
  OP_EXE = 'x',
  OP_VAR = 'v',
  OP_GET = 'g',
  OP_SET = 's',
  OP_IFF = 'i',
  OP_IFE = 'e',
  OP_EQU = '=',
} Parser_OpType_t;

////////////////////////////////////////////////////////////
//	High-level API
////////////////////////////////////////////////////////////

// Entry function for supported string expressions
// The TryGet* or TryParse* pattern indicate that the
// function returned
bool Parser_TryEvaluate(char *string, int *result);

////////////////////////////////////////////////////////////
//	Internal API
////////////////////////////////////////////////////////////

// The core function
bool Parser_TryParseExpression(char *string, int *cursor,
                               int *result);

// Shows an error with context to the developer debug
void Parser_ShowError(const char *string, const int cursor);

// Gets an integer from base 10
bool Parser_TryGetInt(char *string, int *cursor,
                      int *result);

// Parses and op code listed from Parser_OpType_t
bool Parser_TryGetOpCode(char *string, int *cursor,
                         Parser_OpType_t *result);

////////////////////////////////////////////////////////////
//	Consumers API
////////////////////////////////////////////////////////////

// Eats spaces and newlines
void Parser_MaybeConsumeWhitespace(char *string,
                                   int *cursor);

// Eats one instance of the given char, otherwise
// returningfalse
bool Parser_TryConsumeThisChar(char *string, int *cursor,
                               char thisChar);

////////////////////////////////////////////////////////////
//	Variables API
////////////////////////////////////////////////////////////

// Checks if char is part of a valid
// variable name 1+ of (A-Z and _)
bool Parser_IsValidCharForVarName(const char charInVar);

// Gets the variable name string
bool Parser_TryGetVariableName(char *string, int *cursor,
                               char **varName);

// Processes the op code found
bool Parser_TryApplyOpCode(char *string, int *cursor,
                           Parser_OpType_t opCode,
                           int *result);

// Consumes all of the input string, failing if otherwise
bool Parser_TryConsumeThiString(char *string, int *cursor,
                                const char *thisString);

////////////////////////////////////////////////////////////
//	Logical operations
////////////////////////////////////////////////////////////

// Applies if-else logic
bool Parser_TryApplyOp_ife(char *string, int *cursor,
                           int *result);

// Applies equ(1,1) logic to the result of the two sub
// expressions
bool Parser_TryApplyOp_equ(char *string, int *cursor,
                           int *result);

#endif  // PARSER_H
