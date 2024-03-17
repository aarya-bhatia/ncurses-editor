#include "edit_node.h"
#include <assert.h>
#include <string.h>

EditNode *edit_node_new(int index)
{
    EditNode *node = calloc(1, sizeof *node);
    if (!node) {
        return NULL;
    }

    node->index = index;
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

    node->capacity = MAX(node->capacity, MAX(node->size + 2, MIN_CAPACITY));
    assert(node->capacity >= MIN_CAPACITY);
    node->buffer = realloc(node->buffer, node->capacity);
    node->buffer[node->size++] = value;
    node->buffer[node->size] = 0;
}

EditNode *edit_node_split(EditNode *node, size_t new_size)
{
	assert(node);

    EditNode *new_node = edit_node_new(node->index + new_size);
    new_node->buffer = strdup(node->buffer + new_size);
    new_node->size = strlen(new_node->buffer);
    new_node->capacity = new_node->size + 1;

    node->buffer = realloc(node->buffer, new_size + 1);
    node->buffer[new_size] = 0;
    node->size = strlen(node->buffer);
    node->capacity = node->size + 1;

    return new_node;
}

