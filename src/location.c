#include "include/location.h"
#include <string.h>

#define LOCATION_NULL                                                                                                  \
    (Location)                                                                                                         \
    {                                                                                                                  \
        NULL, NULL                                                                                                     \
    }

#define OFFSET(location) ((location).ptr - (location).node->buffer)

bool location_ok(Location l)
{
    return l.node != NULL && l.node->buffer != NULL && l.ptr != NULL && l.ptr >= l.node->buffer &&
           l.ptr < l.node->buffer + l.node->size;
}

ssize_t location_to_index(Location l)
{
    if (!location_ok(l)) {
        return -1;
    }

    EditNode *itr = l.node;
    size_t accum = OFFSET(l);

    while ((itr = itr->prev) != NULL) {
        accum += itr->size;
    }

    return accum;
}

Location location_next_byte(Location l)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    if (l.ptr[1] != 0) {
        return (Location){l.node, l.ptr + 1};
    }

    EditNode *node = l.node->next;

    if (!node || !node->buffer || node->size == 0) {
        return LOCATION_NULL;
    }

    return (Location){node, node->buffer};
}

Location location_prev_byte(Location l)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    if (OFFSET(l) > 0) {
        return (Location){l.node, l.ptr - 1};
    }

    EditNode *node = l.node->prev;

    if (!node || !node->buffer || node->size == 0) {
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

    l = location_find_end_word(l);
    Location next = location_next_byte(l);
    if (!location_ok(next)) {
        return l;
    }

    Location prev = next;
    while (location_ok(next) && char_type(next.ptr[0]) == C_WHITESPACE) {
        prev = next;
        next = location_next_byte(next);
    }

    if (!location_ok(next)) {
        return prev;
    }

    return next;
}

Location location_prev_word(Location l)
{
    if (!location_ok(l)) {
        return LOCATION_NULL;
    }

    Location begin = location_find_begin_word(l);
    if (memcmp(&begin, &l, sizeof l) != 0) {
        return begin;
    }

    Location prev = location_prev_byte(begin);
    Location next = begin;
    while (location_ok(prev) && char_type(prev.ptr[0]) == C_WHITESPACE) {
        next = prev;
        prev = location_prev_byte(prev);
    }

    if (!location_ok(prev)) {
        return next;
    }

    return location_find_begin_word(prev);
}
