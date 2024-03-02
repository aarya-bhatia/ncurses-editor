#include "gap_buffer.h"
#include <string.h>

void freegapbuffer(gapbuffer_t *b) {
  if (b != NULL && b->_buffer != NULL) {
    free(b->_buffer);
  }

  memset(b, 0, sizeof *b);
}

void _gapbuffer_reserve(gapbuffer_t *b, size_t size) {
  if (size <= b->_capacity) {
    return;
  }

  b->_buffer = realloc(b->_buffer, size);
  b->_capacity = size;
}

void _gapbuffer_flush_gap(gapbuffer_t *b) {
  if (!b->_gap_enable) {
    return;
  }

  if (b->_gap_used < b->_gap_size) {
    size_t size_after = b->_size - b->_gap_size - b->_gap_start;
    if (size_after) {
      memmove(b->_buffer + b->_gap_start + b->_gap_used,
              b->_buffer + b->_gap_start + b->_gap_size,
              b->_size - b->_gap_size);
    }
  }

  b->_size += b->_gap_size;
  b->_gap_size = b->_gap_used = 0;
  b->_gap_enable = false;
}

void _gapbuffer_create_gap(gapbuffer_t *b, size_t start, size_t size) {
  if (size == 0) {
    return;
  }

  _gapbuffer_flush_gap(b);
  _gapbuffer_reserve(b, b->_size + size);

  memmove(b->_buffer + start + size, b->_buffer + start, b->_size - start);

  b->_gap_enable = true;
  b->_gap_start = start;
  b->_gap_size = size;
  b->_gap_used = 0;
}
