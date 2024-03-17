#include "edit_node.h"
#include <assert.h>

EditNode *edit_node_new()
{
    return calloc(1, sizeof(EditNode));
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

    node->capacity = MAX(node->capacity, MAX(node->size + 2, MIN_CAPACITY));
    assert(node->capacity >= MIN_CAPACITY);
    node->buffer = realloc(node->buffer, node->capacity);
    node->buffer[node->size++] = value;
    node->buffer[node->size] = 0;
}
