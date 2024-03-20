#include "location.h"
#include "motions.h"
#include <assert.h>
#include <string.h>

Location LOCATION_NONE = {NULL, -1, -1};

Location location(EditNode *container, int offset, int index)
{
    return (Location){container, offset, index};
}

bool has_next_node(EditBuffer *b, Location current)
{
    return LIST_END(b) != current.container->next;
}

bool has_prev_node(EditBuffer *b, Location current)
{
    return LIST_BEGIN(b) != current.container;
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
    assert(memcmp(&current, &LOCATION_NONE, sizeof(Location)) != 0);

    int cur_type = char_type(current.container->buffer[current.offset]);
    if (cur_type == C_WHITESPACE) {
        return find_next_character(b, current, ~C_WHITESPACE);
    }

    Location found = find_next_character(b, current, ~cur_type | C_WHITESPACE);
    if (found.container == NULL) {
        return found;
    }

    int found_type = char_type(found.container->buffer[found.offset]);
    if (found_type == C_WHITESPACE) {
        return find_next_character(b, found, ~C_WHITESPACE);
    }

    return found;
}
