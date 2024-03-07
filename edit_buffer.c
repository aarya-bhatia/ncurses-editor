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
 * Ensures buffer gap is at least `size` bytes wide
 */
void _edit_buffer_gap_reserve(EditBuffer *b, size_t size)
{
    if (b->gap_size >= size) {
        return;
    }

    _edit_buffer_gap_resize(b, align_size(size));
}

/**
 * Resize gap to _exactly_ `size` bytes and possibly reallocates buffer.
 */
void _edit_buffer_gap_resize(EditBuffer *b, size_t size)
{
    // calc new capacity after resizing gap to `size` bytes
    if (b->capacity - b->gap_size + size > b->capacity) {
        b->capacity = b->capacity - b->gap_size + size;
        b->buffer = realloc(b->buffer, b->capacity);
    }

    // move bytes on right of old gap, to be on right of new gap
    size_t msize = b->capacity - b->gap_start - b->gap_size;
    memmove(b->buffer + b->gap_start + size, b->buffer + b->gap_start + b->gap_size, msize);

    b->gap_size = size;
}

/**
 * Set insert position to the index-th character of the buffer.
 * Note that, 0 <= index <= used
 */
void edit_buffer_set_position(EditBuffer *b, size_t index)
{
    assert(index <= b->used);

    // nothing to be done
    if (index == b->gap_start) {
        return;
    }

    // collapse the current gap
    _edit_buffer_gap_resize(b, b->gap_used);
    assert(b->gap_size == b->gap_used);

    // available space in buffer
    size_t avail = b->capacity - b->used;

    // create gap with all free space in buffer at the given index
    assert(b->capacity >= index + avail);
    memmove(b->buffer + index + avail, b->buffer + index, b->used - index);

    b->gap_start = index;
    b->gap_used = 0;
    b->gap_size = avail;
}

/**
 * Insert character at current position in the buffer gap
 */
void edit_buffer_insert(EditBuffer *b, char value)
{
    // ensure space for next byte in gap
    _edit_buffer_gap_reserve(b, b->gap_used + 1);
    b->buffer[b->gap_start + b->gap_used] = value;
    b->gap_used++;
    b->used++;
}

/**
 * Erase and empty buffer
 */
void edit_buffer_clear(EditBuffer *b)
{
    // make buffer one big gap
    b->gap_size = b->capacity;
    b->gap_used = 0;
    b->gap_start = 0;
    b->used = 0;
}

/**
 * Flushes the gap and null terminates buffer.
 * Internally, the gap is shifted to the end of the buffer.
 */
const char *edit_buffer_flush(EditBuffer *b)
{
    // move cursor to last character of buffer
    edit_buffer_set_position(b, b->used);
    assert(b->gap_start == b->used);
    assert(b->gap_used == 0);

    // create new gap at cursor
    _edit_buffer_gap_reserve(b, 1);
    assert(b->gap_size > 0);

    b->buffer[b->used] = 0; // null terminate the buffer
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
