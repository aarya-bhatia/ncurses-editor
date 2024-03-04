#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

void edit_buffer_free(EditBuffer *b) {
  if (b == NULL) {
    return;
  }

  if (b->buffer != NULL) {
    free(b->buffer);
  }

  memset(b, 0, sizeof *b);
}

void edit_buffer_reserve(EditBuffer *b, size_t size) {
  assert(b);
  if (size <= b->capacity) {
    return;
  }

  int new_size = 1;
  while (new_size < size) {
    new_size <<= 1;
  }

  b->buffer = realloc(b->buffer, new_size);
  b->capacity = new_size;
}

void edit_buffer_close_gap(EditBuffer *b) {
  assert(b);
  if (!b->gap_enable) {
    return;
  }

  if (b->gap_used < b->gap_size) {
    memmove(b->buffer + b->gap_start + b->gap_used,
            b->buffer + b->gap_start + b->gap_size,
            b->size - b->gap_size - b->gap_start);
  }

  b->size -= b->gap_size - b->gap_used;
  b->gap_size = b->gap_used = 0;
  b->gap_enable = false;
}

void edit_buffer_open_gap(EditBuffer *b, size_t start, size_t size) {
  assert(b);
  if (size == 0) {
    return;
  }

  edit_buffer_close_gap(b);
  edit_buffer_reserve(b, b->size + size);

  memmove(b->buffer + start + size, b->buffer + start, b->size - start);

  b->gap_enable = true;
  b->gap_start = start;
  b->gap_size = size;
  b->gap_used = 0;
  b->size += size;
}

void edit_buffer_gap_resize(EditBuffer *b, size_t new_size) {
  assert(b);
  assert(b->gap_enable);
  if (new_size <= b->gap_size) {
    return;
  }

  size_t old_size = b->gap_size;
  edit_buffer_reserve(b, b->size - old_size + new_size);
  memmove(b->buffer + b->gap_start + new_size,
          b->buffer + b->gap_start + old_size,
          b->size - b->gap_start - old_size);

  b->size += new_size - old_size;
  b->gap_size = new_size;
}

void edit_buffer_gap_clear(EditBuffer *b) {
  assert(b);
  if (!b->gap_enable) {
    return;
  }

  b->gap_used = 0;
  edit_buffer_close_gap(b);
}

void edit_buffer_insert(EditBuffer *b, int c) {
  assert(b);

  if (b->gap_enable) {
    if (b->gap_used >= b->gap_size) {
      edit_buffer_gap_resize(b, b->gap_size * 2);
    }
    (b->buffer + b->gap_start)[b->gap_used++] = c;
  } else {
    edit_buffer_reserve(b, b->size + 1);
    b->buffer[b->size++] = c;
  }
}

void edit_buffer_backspace(EditBuffer *b) {
  assert(b);

  if (b->gap_enable) {
    if (b->gap_used > 0) {
      b->gap_used--;
    } else if (b->gap_start > 0) {
      b->gap_start--;
      b->gap_size++;
    }
  } else {
    b->size--;
  }
}

void edit_buffer_print_string(EditBuffer *b, printer p) {
  assert(b);

  edit_buffer_reserve(b, b->size + 1);

  if (!b->gap_enable) {
    b->buffer[b->size] = 0;
    p("%s", b->buffer);
  } else {
    if (b->gap_used == b->gap_size) {
      b->buffer[b->size] = 0;
      p("%s", b->buffer);
    } else {
      b->buffer[b->gap_start + b->gap_used] = 0;
      p("%s", b->buffer);
      b->buffer[b->size] = 0;
      p("%s", b->buffer + b->gap_start + b->gap_size);
    }
  }
}
