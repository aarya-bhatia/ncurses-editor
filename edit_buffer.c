#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

void edit_buffer_init(EditBuffer *b)
{
    memset(b, 0, sizeof *b);
}

EditNode *edit_buffer_find_insert_node(EditBuffer *b, size_t index)
{
    size_t accum = 0;
    for (EditNode *node = b->head; node; node = node->next) {
        if (accum <= index && index < accum + node->size) {
            return node;
        }
    }

    return NULL;
}

void edit_buffer_set_insert_position(EditBuffer *b, size_t index)
{
    if (!b->head && !b->tail) {
        EditNode *new_node = calloc(1, sizeof *new_node);
        b->head = b->tail = new_node;
        b->current = new_node;
        return;
    }

    EditNode *node = edit_buffer_find_insert_node(b, index);

    if (!node) {
        EditNode *new_node = calloc(1, sizeof *new_node);
        new_node->index = b->tail->index + b->tail->size;
        b->tail->next = new_node;
        b->tail = new_node;
        b->current = new_node;
        return;
    }

    EditNode *new_node = calloc(1, sizeof *new_node);
    node->next = new_node;
    new_node->next = node->next;
    new_node->buffer = strdup(node->buffer + index - node->index);
    new_node->size = strlen(new_node->buffer);
    new_node->capacity = new_node->size + 1;

    node->buffer = realloc(node->buffer, index - node->index + 1);
    node->buffer[index - node->index] = 0;
    node->size = strlen(node->buffer);
    node->capacity = node->size + 1;

    new_node->index = node->index + node->size;
    b->current = new_node;
}

size_t edit_buffer_size(EditBuffer *b)
{
    size_t len = 0;
    for (EditNode *node = b->head; node; node = node->next) {
        len += node->size;
    }
    return len;
}

char *edit_buffer_to_string(EditBuffer *b)
{
    size_t len = edit_buffer_size(b);
    char *res = calloc(len + 1, 1);
    size_t res_idx = 0;
    for (EditNode *node = b->head; node; node = node->next) {
        memcpy(res + res_idx, node->buffer, node->size);
        res_idx += node->size;
    }

    return res;
}

void edit_buffer_insert(EditBuffer *b, char value)
{
    if (b->current->size + 1 >= b->current->capacity) {
        b->current->capacity = (b->current->capacity + 1) * 2;
        b->current->buffer = realloc(b->current->buffer, b->current->capacity);
    }

    b->current->buffer[b->current->size++] = value;
    b->current->buffer[b->current->size] = 0;
}

void edit_buffer_clear(EditBuffer *b)
{
	EditNode *tmp = NULL;
    for (EditNode *node = b->head; node; node = tmp) {
		tmp = node->next;
        free(node->buffer);
        free(node);
    }

    b->head = b->tail = b->current = NULL;
}

void edit_buffer_free(EditBuffer *b)
{
    if (b) {
        edit_buffer_clear(b);
    }
}
