#include "edit_buffer.h"
#include "location.h"
#include <assert.h>
#include <string.h>

void test0()
{
    EditBuffer b;
    edit_buffer_init(&b);

    // 3 'words' in 1 node: hello - aarya - !
    edit_buffer_insert(&b, 'h');
    edit_buffer_insert(&b, 'e');
    edit_buffer_insert(&b, 'l');
    edit_buffer_insert(&b, 'l');
    edit_buffer_insert(&b, 'o');
    edit_buffer_insert(&b, ' ');
    edit_buffer_insert(&b, 'a');
    edit_buffer_insert(&b, 'a');
    edit_buffer_insert(&b, 'r');
    edit_buffer_insert(&b, 'y');
    edit_buffer_insert(&b, 'a');
    edit_buffer_insert(&b, '!');

    Location current = location(LIST_BEGIN(&b), 0, 0);
    Location next = get_forward_adjacent(&b, current);
    assert(next.container == LIST_BEGIN(&b));
    assert(next.offset == 1);
    assert(next.index == 0);

    Location prev = get_backward_adjacent(&b, next);
    assert(!memcmp(&prev, &current, sizeof prev));

    edit_buffer_clear(&b);
}

void test1()
{
    char *str = "Hello World";
    EditBuffer b;
    edit_buffer_init(&b);
    for (int i = 0; i < strlen(str); i++) {
        edit_buffer_insert(&b, str[i]);
    }

    char *s = edit_buffer_to_string(&b);
    assert(!strcmp(s, str));
    free(s);

    Location hello = location(LIST_BEGIN(&b), 0, 0);
    Location world = find_next_word(&b, hello);

    assert(world.index == 0);
    assert(world.offset == 6);
    assert(world.container == hello.container);
}

int main()
{
    test0();
    test1();
    return 0;
}
