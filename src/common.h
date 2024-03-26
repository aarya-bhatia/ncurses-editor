#pragma once

#include <ctype.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum { C_UNDEFINED = 1, C_WHITESPACE = 2, C_ALPHANUM = 4, C_SPECIAL = 8, C_CONTROL = 16 };

int char_type(char c);
