#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

/**
 * constructor
 */
void edit_buffer_init(EditBuffer *b)
{
    memset(b, 0, sizeof *b);

    b->head.prev = NULL;
    b->tail.next = NULL;
    b->head.next = &b->tail;
    b->tail.prev = &b->head;
    b->current = NULL;
}

/**
 * empty the buffer
 */
void edit_buffer_clear(EditBuffer *b)
{
    EditNode *tmp = NULL;
    for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = tmp) {
        tmp = node->next;
        edit_node_free(node);
    }

    edit_buffer_init(b);
}

/**
 * returns number of bytes in buffer
 */
size_t edit_buffer_size(EditBuffer *b)
{
    size_t len = 0;
    EditNode *node;
    LIST_FOR_EACH(b, node, { len += node->size; });
    return len;
}

/**
 * To delete a double linked node
 */
void edit_buffer_delete_node(EditBuffer *b, EditNode *node)
{
    // sanity checks
    assert(node);
    assert(node != &b->head);
    assert(node != &b->tail);
    assert(node->prev);
    assert(node->next);

    if (node == b->current) {
        b->current = NULL;
    }

    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->prev = node->next = NULL;
    edit_node_free(node);
}

/**
 * To insert a double linked node between two nodes
 */
void edit_buffer_insert_node(EditBuffer *b, EditNode *before, EditNode *insert, EditNode *after)
{
    // sanity checks
    assert(before);
    assert(insert);
    assert(after);
    assert(before != insert);
    assert(insert != after);
    assert(before != after);
    assert(before->next == after);
    assert(after->prev == before);

    before->next = insert;
    insert->prev = before;
    insert->next = after;
    after->prev = insert;
}

/**
 * returns buffer as string
 */
char *edit_buffer_to_string(EditBuffer *b)
{
    size_t len = edit_buffer_size(b);
    char *res = calloc(len + 1, 1);
    size_t res_idx = 0;
    EditNode *node;
    LIST_FOR_EACH(b, node, {
        memcpy(res + res_idx, node->buffer, node->size);
        res_idx += node->size;
    });

    return res;
}

/**
 * add node to tail
 */
EditNode *edit_buffer_append_node(EditBuffer *b, EditNode *node)
{
    assert(b);
    assert(node);
    edit_buffer_insert_node(b, LIST_END(b)->prev, node, LIST_END(b));
    return node;
}

/**
 * insert byte at current position in buffer
 */
void edit_buffer_insert(EditBuffer *b, char value)
{
    assert(b);
    if (!b->current) {
        edit_buffer_set_insert_position(b, 0);
    }
    assert(b->current);
    edit_node_append(b->current, value);
}

/**
 * set current buffer position at the index-th byte in buffer
 * returns the current edit node
 */
EditNode *edit_buffer_set_insert_position(EditBuffer *b, size_t index)
{
    if (LIST_EMPTY(b)) {
        EditNode *first = edit_node_new();
        edit_buffer_append_node(b, first);
        return b->current = first;
    }

    if (index >= edit_buffer_size(b)) {
        return b->current = LIST_END(b)->prev;
    }

    size_t accum = 0;
    EditNode *node;

    LIST_FOR_EACH(b, node, {
        if (index == accum + node->size) {
            return b->current = node;
        } else if (index >= accum && index < accum + node->size) {
            size_t offset = index - accum;

            EditNode *split_node = calloc(1, sizeof *split_node);
            edit_buffer_insert_node(b, node, split_node, node->next);

            // keep the bytes upto index in node and move the rest to split_node
            split_node->buffer = strdup(node->buffer + offset);
            split_node->size = strlen(split_node->buffer);
            split_node->capacity = split_node->size + 1;

            node->buffer[offset] = 0;
            node->size = strlen(node->buffer);

            return b->current = node;
        }

        accum += node->size;
    });

    assert(0); // never come here
    return NULL;
}

/**
 * delete previously inserted byte from buffer
 */
void edit_buffer_backspace(EditBuffer *b)
{
    if (!b->current) {
        return;
    }

    // erase one byte from current edit node
    if (b->current->size > 0) {
        b->current->buffer[b->current->size - 1] = 0;
        b->current->size--;
        return;
    }

    // no more bytes to erase backwards
    if (LIST_IS_FIRST(b, b->current)) {
        return;
    }

    // save prev node temporarily
    EditNode *prev = b->current->prev;

    // remove current node as it is empty
    edit_buffer_delete_node(b, b->current);

    // recurse into previous node
    b->current = prev;
    edit_buffer_backspace(b);
}

/**
 * delete all bytes from beginning till cursor
 */
void edit_buffer_clear_till_beginning(EditBuffer *b)
{
    if (!b->current) {
        return;
    }

    // do not delete the first node
    if (LIST_IS_FIRST(b, b->current)) {
        edit_node_clear(b->current);
        return;
    }

    // save node temporarily
    EditNode *prev = b->current->prev;

    // erase current node
    edit_buffer_delete_node(b, b->current);
    b->current = prev;
    edit_buffer_clear_till_beginning(b);
}
