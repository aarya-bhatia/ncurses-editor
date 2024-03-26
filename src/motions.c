#include "include/edit_buffer.h"
#include "include/location.h"

Location get_location(EditBuffer *b, int index)
{
    size_t accum = 0;
    for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = node->next) {
        if (index >= accum && index < accum + node->size) {
            return (Location){node, node->buffer + (index - accum)};
        }
        accum += node->size;
    }

    return (Location){NULL, NULL};
}

int edit_buffer_get_first_nonspace(EditBuffer *b)
{
    EditNode *begin = LIST_BEGIN(b);
    Location l = (Location){begin, begin->buffer};
    Location found = location_find_forward(l, ~C_WHITESPACE);
    if (!location_ok(found)) {
        return 0;
    }

    return location_to_index(found);
}

int edit_buffer_get_prev_word(EditBuffer *b, int current)
{
    Location l = get_location(b, current);
    if (!location_ok(l)) {
        return -1;
    }

    Location found = location_prev_word(l);
    if (!location_ok(found)) {
        return current;
    }

    return location_to_index(found);
}

int edit_buffer_get_next_word(EditBuffer *b, int current)
{
    Location l = get_location(b, current);
    if (!location_ok(l)) {
        return -1;
    }

    Location found = location_next_word(l);
    if (!location_ok(found)) {
        Location end = location_find_end_word(l);
        return location_to_index(end);
    }

    return location_to_index(found);
}

int edit_buffer_get_end_word(EditBuffer *b, int current)
{
    Location l = get_location(b, current);
    if (!location_ok(l)) {
        return -1;
    }

    Location end = location_find_end_word(l);
	if(end.ptr == l.ptr) {
		Location next = location_next_word(l);
		if(!location_ok(next)){
			return location_to_index(end);
		} else {
			return location_to_index(location_find_end_word(next));
		}
	}

    return location_to_index(end);
}
