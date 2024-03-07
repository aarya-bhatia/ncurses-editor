#include "edit_buffer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * Make size be the nearest power of 2
 */
size_t align_size(size_t size)
{
    size_t new_size = 1;
    while (new_size < size) {
        new_size <<= 1;
    }

    return new_size;
}

/**
 * Reserve gap space in the buffer and possibly reallocate it.
 */
void edit_buffer_gap_reserve(EditBuffer *b, size_t size)
{
    size = align_size(size);
    if (b->gap_size >= size) {
        return;
    }

    b->buffer = realloc(b->buffer, b->capacity - b->gap_size + size);
    b->capacity = b->capacity - b->gap_size + size;

    memmove(b->buffer + b->gap_start + size, b->buffer + b->gap_start + b->gap_size,
            b->capacity - b->gap_start - b->gap_size);

    b->gap_size = size;
}

/**
 * Set insert position to the index-th character of the buffer.
 */
size_t edit_buffer_set_position(EditBuffer *b, size_t index)
{
    size_t used = b->capacity - b->gap_size + b->gap_used;

    // translate index so that it is gap aware
    assert(index <= used);
    if (index >= b->gap_start + b->gap_used) {
        size_t back_index = (index - b->gap_used - b->gap_start);
        index = b->capacity - back_index;
        fprintf(stderr, "index=%zu\n", index);
    }

    // available space in buffer
    size_t avail = b->gap_size - b->gap_used;

    // collpase prev gap at position `gap_start` of size `gap_size`
    memmove(b->buffer + b->gap_start + b->gap_used, b->buffer + b->gap_start + b->gap_size,
            b->capacity - b->gap_start - b->gap_size);

    // create new gap of size `avail` at position `index`
    memmove(b->buffer + index + avail, b->buffer + index, b->capacity - index - avail);

    b->gap_start = index;
    b->gap_used = 0;
    b->gap_size = avail;

    return index;
}

/**
 * Insert character at current position in the buffer gap
 */
void edit_buffer_insert(EditBuffer *b, char value)
{
    // ensure space for new byte in current gap
    edit_buffer_gap_reserve(b, b->gap_used + 1);
    b->buffer[b->gap_start + b->gap_used] = value;
    b->gap_used++;
}

/**
 * Erase and empty buffer
 */
void edit_buffer_clear(EditBuffer *b)
{
    // entire buffer is one big gap
    b->gap_size = b->capacity;
    b->gap_used = 0;
    b->gap_start = 0;
}

/**
 * Flushes the gap and null terminates buffer.
 * Internally, the gap is shifted to the end of the buffer.
 */
const char *edit_buffer_flush(EditBuffer *b)
{
    size_t used = b->capacity - b->gap_size + b->gap_used;
    edit_buffer_set_position(b, used);
    edit_buffer_gap_reserve(b, 1);
    b->buffer[b->gap_start + b->gap_used] = 0;
    return b->buffer;
}

/**
 * Destructor
 */
void edit_buffer_free(EditBuffer *b)
{
    if (b) {
        free(b->buffer);
        memset(b, 0, sizeof *b);
    }
}

// void edit_buffer_print_window(EditBuffer *b, WINDOW *win)
// {
//     assert(b);
//     assert(win);
//     werase(win);
//     edit_buffer_reserve(b, b->size + 1);
//     b->buffer[b->size] = 0;
//
//     if (b->gap_used < b->gap_size) {
//         b->buffer[b->gap_used] = 0;
//         // print string before gap
//         wprintw(win, "%s", b->buffer);
//         // print string after gap
//         wprintw(win, "%s", b->buffer + b->gap_start + b->gap_size);
//     } else {
//         // no gaps present
//         wprintw(win, "%s", b->buffer);
//     }
//
//     wrefresh(win);
// }
//
