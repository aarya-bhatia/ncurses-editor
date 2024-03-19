#include "motions.h"
#include <ctype.h>

int edit_buffer_get_first_nonspace(EditBuffer *b)
{
    int cur = 0;
    for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = node->next) {
        for (int i = 0; i < node->size; i++) {
            if (!isspace(node->buffer[i])) {
                cur += i;
                return cur;
            }
        }
        cur += node->size;
    }

    return cur;
}

// TODO
int edit_buffer_get_prev_word(EditBuffer *b, int current)
{
    return -1;
}

// TODO
int edit_buffer_get_next_word(EditBuffer *b, int current)
{
    return -1;
}

// TODO
int edit_buffer_get_end_word(EditBuffer *b, int current)
{
    return -1;
}
