#include "location.h"
#include <assert.h>

#define LOCATION_NULL                                                                                                  \
    (Location)                                                                                                         \
    {                                                                                                                  \
        NULL, NULL                                                                                                     \
    }

#define IS_SENITEL(node) ((node)->prev == NULL || (node)->next == NULL)
#define OFFSET(location) ((location).ptr - (location).node->buffer)

bool location_ok(Location l)
{
    bool node_ok = l.node != NULL && l.node->prev != NULL && l.node->next != NULL;
    bool ptr_ok = l.ptr != NULL && l.ptr >= l.node->buffer && l.ptr < l.node->buffer + l.node->size;
    return node_ok && ptr_ok;
}

// TODO
Location location_at(EditBuffer *b, size_t index)
{
    return LOCATION_NULL;
}

size_t location_to_index(Location l)
{
    assert(location_ok(l));

    EditNode *itr = l.node;
    size_t accum = OFFSET(l);

    while (!IS_SENITEL(itr->prev)) {
        itr = itr->prev;
        accum += itr->size;
    }

    return accum;
}

Location location_next_byte(Location l)
{
    assert(location_ok(l));

    if (l.ptr[1] != 0) {
        return (Location){l.node, l.ptr + 1};
    }

    EditNode *node = l.node->next;

    if (IS_SENITEL(node) || node->size == 0) {
        return LOCATION_NULL;
    }

    return (Location){node, node->buffer};
}

Location location_prev_byte(Location l)
{
    assert(location_ok(l));

    if (OFFSET(l) > 0) {
        return (Location){l.node, l.ptr - 1};
    }

    EditNode *node = l.node->prev;

    if (IS_SENITEL(node) || node->size == 0) {
        return LOCATION_NULL;
    }

    return (Location){node, node->buffer + node->size - 1};
}

Location location_find_forward(Location l, int type_mask)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    if (char_type(*l.ptr) & type_mask) {
        return l;
    }

    return location_find_forward(location_next_byte(l), type_mask);
}

Location location_find_backward(Location l, int type_mask)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    if (char_type(*l.ptr) & type_mask) {
        return l;
    }

    return location_find_backward(location_prev_byte(l), type_mask);
}

Location location_find_end_word(Location l)
{
    int type = char_type(l.ptr[0]);
    Location prev = l;
    for (Location itr = l; location_ok(itr); itr = location_next_byte(itr)) {
        if (char_type(itr.ptr[0]) != type) {
            return prev;
        }
        prev = itr;
    }

    return prev;
}

Location location_find_begin_word(Location l)
{
    int type = char_type(l.ptr[0]);
    Location next = l;
    for (Location itr = l; location_ok(itr); itr = location_prev_byte(itr)) {
        if (char_type(itr.ptr[0]) != type) {
            return next;
        }
        next = itr;
    }

    return next;
}

Location location_next_word(Location l)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    Location found = location_find_forward(l, ~char_type(l.ptr[0])); // find a separator
    if (!location_ok(found)) {
        return l;
    }

    if (char_type(found.ptr[0]) == C_WHITESPACE) {
        found = location_find_forward(found, ~C_WHITESPACE); // skip whitespace
        if (!location_ok(found)) {
            return l;
        }
    }

    return found;
}

Location location_prev_word(Location l)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    Location found = location_find_backward(l, ~char_type(l.ptr[0])); // find a separator
    if (!location_ok(found)) {
        return l;
    }

    if (char_type(found.ptr[0]) == C_WHITESPACE) {
        found = location_find_backward(found, ~C_WHITESPACE); // skip whitespace
        if (!location_ok(found)) {
            return l;
        }
    }

    return location_find_begin_word(found);
}
