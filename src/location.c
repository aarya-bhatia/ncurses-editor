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

Location location_next_node(Location l)
{
    assert(location_ok(l));

    if (IS_SENITEL(l.node) || l.node->next->size == 0) {
        return LOCATION_NULL;
    }

    return (Location){l.node->next, l.node->next->buffer};
}

Location location_prev_node(Location l)
{
    assert(location_ok(l));

    if (IS_SENITEL(l.node) || l.node->prev->size == 0) {
        return LOCATION_NULL;
    }

    return (Location){l.node->prev, l.node->prev->buffer};
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

Location location_next_word(Location l)
{
    assert(location_ok(l));
    return LOCATION_NULL; // TODO
}

Location location_prev_word(Location l)
{
    assert(location_ok(l));
    return LOCATION_NULL; // TODO
}

// Location LOCATION_NONE = {NULL, -1, -1};
//
// Location location(EditNode *container, int offset, int index)
// {
//     return (Location){container, offset, index};
// }
//
// Location get_forward_adjacent(EditBuffer *b, Location current)
// {
//     if (current.offset < current.container->size) {
//         return location(current.container, current.offset + 1, current.index);
//     }
//
//     if (!has_next_node(b, current)) {
//         return LOCATION_NONE;
//     }
//
//     return location(current.container->next, 0, current.index + current.container->size);
// }
//
// Location get_backward_adjacent(EditBuffer *b, Location current)
// {
//     if (current.offset > 0) {
//         return location(current.container, current.offset - 1, current.index);
//     }
//
//     if (!has_prev_node(b, current)) {
//         return LOCATION_NONE;
//     }
//
//     return location(current.container->prev, current.container->prev->size - 1,
//                     current.index - current.container->prev->size);
// }
//
// Location find_container_node(EditBuffer *b, int index)
// {
//     int accum = 0;
//
//     for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = node->next) {
//         if (index >= accum && index <= node->size) {
//             return location(node, index - accum, accum);
//         }
//         accum += node->size;
//     }
//
//     return LOCATION_NONE;
// }
//
// Location find_next_character(EditBuffer *b, Location current, int type_mask)
// {
//     for (int i = current.offset; i < current.container->size; i++) {
//         if (char_type(current.container->buffer[i]) & type_mask) {
//             return location(current.container, i, current.index);
//         }
//     }
//
//     if (current.container == LIST_END(b)) {
//         return LOCATION_NONE;
//     }
//
//     return find_next_character(b, location(current.container->next, 0, current.index + current.container->size),
//                                type_mask);
// }
//
// Location find_next_word(EditBuffer *b, Location current)
// {
//     Location found =
//         find_next_character(b, current, ~char_type(byte_at_location(current))); // next character of different type
//     if (found.container == NULL) {
//         EditNode *last = LIST_END(b)->prev;
//         return location(last, last->size - 1, edit_buffer_size(b) - last->size); // last location
//     }
//     if (char_type(byte_at_location(found)) == C_WHITESPACE) {
//         return find_next_character(b, found, ~C_WHITESPACE); // skip whitespace till next non-whitespace character
//     }
//     return found;
// }
