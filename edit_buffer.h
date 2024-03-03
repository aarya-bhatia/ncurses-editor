#pragma once

#include <stdbool.h>
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

