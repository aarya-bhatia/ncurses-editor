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
    struct _EditNode *prev;
} EditNode;

EditNode *edit_node_new();
EditNode *edit_node_new1(const char *str);
void edit_node_free(EditNode *node);
void edit_node_append(EditNode *node, char value);
void edit_node_clear(EditNode *node);
