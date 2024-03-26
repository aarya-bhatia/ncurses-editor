#include "edit_node.h"

typedef struct Location {
    EditNode *node;
    char *ptr; // pointer to node->buffer
} Location;

bool location_ok(Location l);
ssize_t location_to_index(Location l);
Location location_next_byte(Location l);
Location location_prev_byte(Location l);
Location location_find_begin_word(Location l);
Location location_find_end_word(Location l);
Location location_next_word(Location current);
Location location_prev_word(Location current);
Location location_find_forward(Location current, int type_mask);
Location location_find_backward(Location current, int type_mask);
