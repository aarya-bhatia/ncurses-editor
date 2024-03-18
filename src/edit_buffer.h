#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "edit_node.h"

typedef struct EditBuffer {
    EditNode head;
    EditNode tail;
    EditNode *current;
} EditBuffer;

#define LIST_BEGIN(edit_buffer) (edit_buffer)->head.next
#define LIST_END(edit_buffer) &(edit_buffer)->tail

#define LIST_EMPTY(edit_buffer) ((edit_buffer)->head.next == &(edit_buffer)->tail)

#define LIST_FOR_EACH(edit_buffer, iterator, callback)                                                                 \
    for (iterator = LIST_BEGIN(edit_buffer); iterator != LIST_END(edit_buffer); iterator = iterator->next)             \
        callback;

void edit_buffer_init(EditBuffer *b);
void edit_buffer_clear(EditBuffer *b);
size_t edit_buffer_size(EditBuffer *b);
void edit_buffer_insert(EditBuffer *b, char value);
char *edit_buffer_to_string(EditBuffer *b);
EditNode *edit_buffer_set_insert_position(EditBuffer *b, size_t index);
EditNode *edit_buffer_append_node(EditBuffer *b, EditNode *node);
void edit_buffer_backspace(EditBuffer *b);
void edit_buffer_clear_till_beginning(EditBuffer *b);

// todo
// const char *edit_buffer_flush(EditBuffer *b);

void edit_buffer_delete_node(EditBuffer *b, EditNode *node);
void edit_buffer_insert_node(EditBuffer *b, EditNode *before, EditNode *insert, EditNode *after);
