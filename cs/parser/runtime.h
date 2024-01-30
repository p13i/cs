#ifndef RUNTIME_H
#define RUNTIME_H

// Initialize or reset the runtime module
void Runtime_Init();
// Used for get(A) to fetch a variable
bool Runtime_TryGetVarValue(const char* varName,
                            int* result);
// Checks if var is set or not
bool Runtime_IsVarAlloc(const char* varName);
// Used to set a var to a value
bool Runtime_TrySetVarToValue(const char* varName,
                              int value);
// Gets the variable ready to be used, if it's available
bool Runtime_TryAllocVar(const char* varName);

#endif  // RUNTIME_H
