#ifndef PARSER_EXPECTATIONS_H
#define PARSER_EXPECTATIONS_H

#define EXPECT_TRUE_OR_ERROR(cond, fmt, ...) \
  if (false == cond) {                       \
    ERROR(fmt, ##__VA_ARGS__);               \
    return false;                            \
  }

#endif  // PARSER_EXPECTATIONS_H
