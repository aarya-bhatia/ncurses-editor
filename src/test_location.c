#include "edit_buffer.h"
#include "location.h"
#include <assert.h>
#include <string.h>

// test next and previous location
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

// test next word with 1 edit node and 2 words
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

    edit_buffer_clear(&b);
}

// test next word with 2 edit nodes and 2 words
void test2()
{
    EditNode n2;
    n2.buffer = strdup("World");
    n2.size = 5;
    n2.capacity = 5;
    n2.next = NULL;

    EditNode n1;
    n1.buffer = strdup("Hello ");
    n1.size = 6;
    n1.capacity = 6;
    n1.next = NULL;

    EditBuffer b;
    edit_buffer_init(&b);

    edit_buffer_append_node(&b, &n1);
    edit_buffer_append_node(&b, &n2);

    assert(edit_buffer_size(&b) == 11);

    char *s = edit_buffer_to_string(&b);
    assert(!strcmp(s, "Hello World"));
    free(s);

    Location hello = location(LIST_BEGIN(&b), 0, 0);
    assert(hello.index == 0);
    assert(hello.offset == 0);
    assert(hello.container == &n1);

    Location world = find_next_word(&b, hello);
    assert(world.index == 6);
    assert(world.offset == 0);
    assert(world.container == &n2);

    Location end = find_next_word(&b, world);
    assert(end.index == 6);
    assert(end.offset == 4);
    assert(end.container == &n2);

	free(n1.buffer);
	free(n2.buffer);
}

int main()
{
    test0();
    test1();
    test2();
    return 0;
}
