#pragma once

#include "common.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_CAPACITY 8

typedef struct _EditNode {
    char *buffer;
    size_t size;
    size_t capacity;
    struct _EditNode *next;
} EditNode;

EditNode *edit_node_new();
void edit_node_free(EditNode *node);
void edit_node_append(EditNode *node, char value);
