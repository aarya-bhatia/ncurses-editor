#pragma once

#include "edit_buffer.h"
#include <ctype.h>

enum { C_UNDEFINED = 1, C_WHITESPACE = 2, C_ALPHANUM = 4, C_SPECIAL = 8, C_CONTROL = 16 };

int char_type(char c);
int edit_buffer_get_prev_word(EditBuffer *b, int current);
int edit_buffer_get_next_word(EditBuffer *b, int current);
int edit_buffer_get_end_word(EditBuffer *b, int current);
