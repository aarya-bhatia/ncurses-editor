#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

/**
 * constructor
 */
void edit_buffer_init(EditBuffer *b)
{
    memset(b, 0, sizeof *b);
}

/**
 * empty the buffer
 */
void edit_buffer_clear(EditBuffer *b)
{
    EditNode *tmp = NULL;

    for (EditNode *node = b->head; node; node = tmp) {
        tmp = node->next;
        edit_node_free(node);
    }

    b->head = b->tail = b->current = NULL;
}

/**
 * returns number of bytes in buffer
 */
size_t edit_buffer_size(EditBuffer *b)
{
    size_t len = 0;
    for (EditNode *node = b->head; node; node = node->next) {
        len += node->size;
    }
    return len;
}

/**
 * returns buffer as string
 */
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

/**
 * add node to tail
 */
EditNode *edit_buffer_append_node(EditBuffer *b, EditNode *node)
{
    assert(b);
    assert(node);

    if (!b->head && !b->tail) {
        b->head = b->tail = node;
    } else {
        b->tail->next = node;
        b->tail = node;
    }

    return node;
}

/**
 * insert byte at current position in buffer
 */
void edit_buffer_insert(EditBuffer *b, char value)
{
    if (!b->current) {
        edit_buffer_set_insert_position(b, 0);
    }
    edit_node_append(b->current, value);
}

/**
 * set current buffer position at the index-th byte in buffer
 * returns the current edit node
 */
EditNode *edit_buffer_set_insert_position(EditBuffer *b, size_t index)
{
    if (!b->head && !b->tail) {
        EditNode *first = edit_node_new();
        edit_buffer_append_node(b, first);
        return b->current = first;
    }

    if (index >= edit_buffer_size(b)) {
        return b->current = b->tail;
    }

    size_t accum = 0;

    for (EditNode *node = b->head; node; node = node->next) {
        if (index == accum + node->size) {
            return b->current = node;
        } else if (index >= accum && index < accum + node->size) {
            EditNode *split_node = calloc(1, sizeof *split_node);
            split_node->next = node->next;
            node->next = split_node;

            size_t offset = index - accum;

            split_node->buffer = strdup(node->buffer + offset);
            split_node->size = strlen(split_node->buffer);
            split_node->capacity = split_node->size + 1;

            node->buffer[offset] = 0;
            node->size = strlen(node->buffer);

            if (node == b->tail) {
                b->tail = split_node;
            }

            return b->current = node;
        }

        accum += node->size;
    }

    assert(0); // never come here
    return NULL;
}

/**
 * delete previously inserted byte from buffer
 */
void edit_buffer_backspace(EditBuffer *b)
{
    if (edit_buffer_size(b) == 0 || !b->current) {
        return;
    }

    if (b->current->size > 0) {
        b->current->buffer[b->current->size - 1] = 0;
        b->current->size--;
        return;
    }

    if (b->tail == b->current && b->head == b->current) {
        return;
    }

    if (b->current == b->head) {
        b->head = b->current->next;
        edit_node_free(b->current);
        b->current = b->head;
        return;
    }

    for (EditNode *node = b->head; node != NULL; node = node->next) {
        if (node->next == b->current) {
            node->next = node->next->next;
            edit_node_free(b->current);
            b->current = node;
            if (b->current == b->tail) {
                b->tail = node;
            }
            edit_buffer_backspace(b);
            return;
        }
    }

    assert(0); // never come here
}
