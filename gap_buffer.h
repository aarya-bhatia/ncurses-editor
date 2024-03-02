#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct _gapbuffer_t {
  char *_buffer;
  size_t _capacity;
  size_t _size;
  size_t _gap_start;
  size_t _gap_size;
  size_t _gap_used;
  bool _gap_enable;
} gapbuffer_t;

void freegapbuffer(gapbuffer_t *b);
void _gapbuffer_reserve(gapbuffer_t *b, size_t size);
void _gapbuffer_flush_gap(gapbuffer_t *b);
void _gapbuffer_create_gap(gapbuffer_t *b, size_t start, size_t size);
