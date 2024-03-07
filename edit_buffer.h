#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct EditBuffer {
    char *buffer;
    size_t capacity;
    size_t gap_start;
    size_t gap_size;
    size_t gap_used;
} EditBuffer;

void edit_buffer_gap_reserve(EditBuffer *b, size_t size);
size_t edit_buffer_set_position(EditBuffer *b, size_t index);
void edit_buffer_insert(EditBuffer *b, char value);
void edit_buffer_clear(EditBuffer *b);
void edit_buffer_free(EditBuffer *b);
const char *edit_buffer_flush(EditBuffer *b);

void edit_buffer_print_window(EditBuffer *b, WINDOW *win);
