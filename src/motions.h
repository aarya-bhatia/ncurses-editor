#pragma once

#include "edit_buffer.h"

int edit_buffer_get_first_nonspace(EditBuffer *b);
int edit_buffer_get_prev_word(EditBuffer *b, int current);
int edit_buffer_get_next_word(EditBuffer *b, int current);
int edit_buffer_get_end_word(EditBuffer *b, int current);
