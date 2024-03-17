#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "edit_node.h"

typedef struct EditBuffer {
    EditNode *head;
    EditNode *tail;
    EditNode *current;
} EditBuffer;

void edit_buffer_init(EditBuffer *b);
void edit_buffer_clear(EditBuffer *b);
size_t edit_buffer_size(EditBuffer *b);
void edit_buffer_insert(EditBuffer *b, char value);
char *edit_buffer_to_string(EditBuffer *b);
EditNode *edit_buffer_set_insert_position(EditBuffer *b, size_t index);
EditNode *edit_buffer_append_node(EditBuffer *b, EditNode *node);

// todo
// const char *edit_buffer_flush(EditBuffer *b);
