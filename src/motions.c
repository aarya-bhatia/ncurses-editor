#include "include/location.h"
#include "include/edit_buffer.h"

int edit_buffer_get_prev_word(EditBuffer *b, int current)
{
    return -1;
}
int edit_buffer_get_next_word(EditBuffer *b, int current)
{
    return -1;
}
int edit_buffer_get_end_word(EditBuffer *b, int current)
{
    return -1;
}
int edit_buffer_get_first_nonspace(EditBuffer *b)
{
    return -1;
}

// int edit_buffer_get_first_nonspace(EditBuffer *b)
// {
//     int cur = 0;
//     for (EditNode *node = LIST_BEGIN(b); node != LIST_END(b); node = node->next) {
//         for (int i = 0; i < node->size; i++) {
//             if (!isspace(node->buffer[i])) {
//                 cur += i;
//                 return cur;
//             }
//         }
//         cur += node->size;
//     }
//
//     return cur;
// }
//
// // TODO
// int edit_buffer_get_prev_word(EditBuffer *b, int current)
// {
//     return -1;
// }
//
// // TODO
// int edit_buffer_get_next_word(EditBuffer *b, int current)
// {
//     Location l = find_container_node(b, current);
//     if (l.container == NULL) {
//         return edit_buffer_size(b) - 1;
//     }
//
//     l = find_next_word(b, l);
//     if (l.container == NULL) {
//         return edit_buffer_size(b) - 1;
//     }
//
//     return l.index + l.offset;
// }
//
// // TODO
// int edit_buffer_get_end_word(EditBuffer *b, int current)
// {
//     return -1;
// }
