#include "edit_buffer.h"

typedef struct Location {
    EditNode *node;
    char *ptr; // pointer to node->buffer
} Location;

bool location_ok(Location l);
Location location_at(EditBuffer *b, size_t index);
size_t location_to_index(Location l);

Location location_next_node(Location l);
Location location_prev_node(Location l);
Location location_next_byte(Location l);
Location location_prev_byte(Location l);

Location location_find_forward(Location current, int type_mask);
Location location_find_backward(Location current, int type_mask);
Location location_next_word(Location current);
Location location_prev_word(Location current);
