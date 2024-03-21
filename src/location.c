#include "location.h"
#include "motions.h"
#include <assert.h>
#include <string.h>

Location LOCATION_NONE = {NULL, -1, -1};

Location location(EditNode *container, int offset, int index)
{
    return (Location){container, offset, index};
}

Location get_forward_adjacent(EditBuffer *b, Location current)
{
    if (current.offset < current.container->size) {
        return location(current.container, current.offset + 1, current.index);
    }

    if (!has_next_node(b, current)) {
        return LOCATION_NONE;
    }

    return location(current.container->next, 0, current.index + current.container->size);
}

Location get_backward_adjacent(EditBuffer *b, Location current)
{
    if (current.offset > 0) {
        return location(current.container, current.offset - 1, current.index);
    }

    if (!has_prev_node(b, current)) {
        return LOCATION_NONE;
    }

    return location(current.container->prev, current.container->prev->size - 1,
                    current.index - current.container->prev->size);
}

Location find_container_node(EditBuffer *b, int index)
{
    int accum = 0;

    for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = node->next) {
        if (index >= accum && index <= node->size) {
            return location(node, index - accum, accum);
        }
        accum += node->size;
    }

    return LOCATION_NONE;
}

Location find_next_character(EditBuffer *b, Location current, int type_mask)
{
    for (int i = current.offset; i < current.container->size; i++) {
        if (char_type(current.container->buffer[i]) & type_mask) {
            return location(current.container, i, current.index);
        }
    }

    if (current.container == LIST_END(b)) {
        return LOCATION_NONE;
    }

    return find_next_character(b, location(current.container->next, 0, current.index + current.container->size),
                               type_mask);
}

Location find_next_word(EditBuffer *b, Location current)
{
    Location found =
        find_next_character(b, current, ~char_type(byte_at_location(current))); // next character of different type
    if (found.container == NULL) {
        EditNode *last = LIST_END(b)->prev;
        return location(last, last->size - 1, edit_buffer_size(b) - last->size); // last location
    }
    if (char_type(byte_at_location(found)) == C_WHITESPACE) {
        return find_next_character(b, found, ~C_WHITESPACE); // skip whitespace till next non-whitespace character
    }
    return found;
}
