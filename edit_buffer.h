#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct _EditNode {
    char *buffer;
    size_t size;
    size_t capacity;
    size_t index;
    struct _EditNode *next;
} EditNode;

typedef struct EditBuffer {
    EditNode *head;
    EditNode *tail;
    EditNode *current;
} EditBuffer;

size_t edit_buffer_size(EditBuffer *b);
void edit_buffer_insert(EditBuffer *b, char value);
void edit_buffer_clear(EditBuffer *b);
void edit_buffer_init(EditBuffer *b);
void edit_buffer_free(EditBuffer *b);
void edit_buffer_set_insert_position(EditBuffer *b, size_t index);
char *edit_buffer_to_string(EditBuffer *b);

// const char *edit_buffer_flush(EditBuffer *b);
