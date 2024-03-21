#include "edit_buffer.h"

typedef struct Location {
    EditNode *container; // the node containing the character
    int offset;          // local index within the node
    int index;           // global index within the buffer
} Location;

#define byte_at_location(location) ((location).container->buffer[(location).offset])
#define has_next_node(editbuffer, location) (LIST_END(editbuffer) != (location).container->next)
#define has_prev_node(editbuffer, location) (LIST_BEGIN(editbuffer) != (location).container)

extern Location LOCATION_NONE;

Location location(EditNode *container, int offset, int index);
Location get_forward_adjacent(EditBuffer *b, Location current);
Location get_backward_adjacent(EditBuffer *b, Location current);
Location find_container_node(EditBuffer *b, int index);
Location find_next_character(EditBuffer *b, Location current, int type_mask);
Location find_next_word(EditBuffer *b, Location current);
