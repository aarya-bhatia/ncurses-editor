#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct EditBuffer {
  char *buffer;
  size_t capacity;
  size_t size;
  size_t gap_start;
  size_t gap_size;
  size_t gap_used;
  bool gap_enable;
} EditBuffer;

void edit_buffer_reserve(EditBuffer *b, size_t size);
void edit_buffer_gap_resize(EditBuffer *b, size_t new_size);

void edit_buffer_gap_clear(EditBuffer *b);
void edit_buffer_open_gap(EditBuffer *b, size_t start, size_t size);
void edit_buffer_close_gap(EditBuffer *b);
void edit_buffer_free(EditBuffer *b);
void edit_buffer_insert(EditBuffer *b, int c);

int edit_buffer_size(EditBuffer *b);
void edit_buffer_clear(EditBuffer *b);
void edit_buffer_backspace(EditBuffer *b);
void edit_buffer_print_window(EditBuffer *b, WINDOW *win);

const char *edit_buffer_to_string(EditBuffer *b);