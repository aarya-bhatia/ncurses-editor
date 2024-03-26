#include "edit_node.h"
#include <assert.h>
#include <string.h>

EditNode *edit_node_new1(const char *str)
{
    EditNode *node = calloc(1, sizeof *node);
    node->buffer = strdup(str);
    node->capacity = strlen(str) + 1;
    node->size = strlen(str);
    return node;
}

EditNode *edit_node_new()
{
    EditNode *node = calloc(1, sizeof *node);
    node->buffer = calloc(MIN_CAPACITY, 1);
    node->capacity = MIN_CAPACITY;
    return node;
}

void edit_node_free(EditNode *node)
{
    if (!node) {
        return;
    }

    free(node->buffer);
    free(node);
}

void edit_node_append(EditNode *node, char value)
{
    assert(node);

    node->capacity = MAX(node->capacity, node->size + 2);
    node->buffer = realloc(node->buffer, node->capacity);
    node->buffer[node->size++] = value;
    node->buffer[node->size] = 0;
}

void edit_node_clear(EditNode *node)
{
    node->capacity = MIN_CAPACITY;
    node->buffer = realloc(node->buffer, MIN_CAPACITY);
    node->size = 0;
    node->buffer[0] = 0;
}
