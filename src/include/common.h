#pragma once

#include <ctype.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum { C_UNDEFINED = 0x100, C_WHITESPACE = 0x200, C_ALPHANUM = 0x400, C_SPECIAL = 0x800, C_CONTROL = 0x1000 };

int char_type(char c);
