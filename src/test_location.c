#include "edit_buffer.h"
#include "location.h"
#include <assert.h>
#include <string.h>

void test0()
{
    // 3 'words' in 1 node: hello - aarya - !
    EditNode *n = edit_node_new1("hello aarya!");

    EditBuffer b;
    edit_buffer_init(&b);
    edit_buffer_append_node(&b, n);

    Location l, l1, l2;

    assert(!location_ok((Location){NULL, NULL}));
    assert(!location_ok((Location){n, n->buffer + n->size}));
    assert(!location_ok((Location){&b.head, b.head.buffer}));
    assert(!location_ok((Location){&b.tail, b.tail.buffer}));

    l = (Location){n, n->buffer};
    assert(location_ok(l));
	assert(location_to_index(l) == 0);

    l1 = location_next_byte(l);
    l2 = location_prev_byte(l);
    assert(l1.node == n);
    assert(l1.ptr == n->buffer + 1);
    assert(l2.node == NULL);
    assert(l2.ptr == NULL);

    l = (Location){n, n->buffer + 2};
	assert(location_to_index(l) == 2);
    l1 = location_next_byte(l);
    l2 = location_prev_byte(l);

    assert(l1.node == n);
    assert(l1.ptr == n->buffer + 3);
    assert(l2.node == n);
    assert(l2.ptr == n->buffer + 1);

    l = (Location){n, n->buffer + n->size - 1};
    l1 = location_next_byte(l);
    l2 = location_prev_byte(l);
    assert(l1.node == NULL);
    assert(l1.ptr == NULL);
    assert(l2.node == n);
    assert(l2.ptr == n->buffer + n->size - 2);

    l = (Location){n, n->buffer + 5};
	assert(location_to_index(l) == 5);
    assert(l.node == n);
    assert(l.ptr[0] == ' ');
    l1 = location_find_forward(l, C_ALPHANUM);
    l2 = location_find_backward(l, C_ALPHANUM);
    assert(l1.ptr[0] == 'a');
    assert(l2.ptr[0] == 'o');
    l1 = location_find_forward(location_next_byte(l1), C_ALPHANUM);
    l2 = location_find_backward(location_prev_byte(l2), C_ALPHANUM);
    assert(l1.ptr[0] == 'a');
    assert(l2.ptr[0] == 'l');

    l = (Location){n, n->buffer};
    l2 = location_find_backward(l, C_ALPHANUM);
    assert(l2.node == n);
    l2 = location_find_backward(location_prev_byte(l2), C_ALPHANUM);
    assert(l2.node == NULL);

    l1 = location_find_forward(l, C_WHITESPACE);
    assert(l1.ptr[0] == ' ');

    l1 = location_find_forward(l, C_SPECIAL);
    assert(l1.ptr[0] == '!');

    edit_buffer_clear(&b);
}

// test next word with 1 edit node and 2 words
// void test1()
// {
//     char *str = "Hello World";
//     EditBuffer b;
//     edit_buffer_init(&b);
//     for (int i = 0; i < strlen(str); i++) {
//         edit_buffer_insert(&b, str[i]);
//     }
//
//     char *s = edit_buffer_to_string(&b);
//     assert(!strcmp(s, str));
//     free(s);
//
//     Location hello = location(LIST_BEGIN(&b), 0, 0);
//     Location world = find_next_word(&b, hello);
//
//     assert(world.index == 0);
//     assert(world.offset == 6);
//     assert(world.container == hello.container);
//
//     edit_buffer_clear(&b);
// }

// test next word with 2 edit nodes and 2 words
// void test2()
// {
//     EditNode n2;
//     n2.buffer = strdup("World");
//     n2.size = 5;
//     n2.capacity = 5;
//     n2.next = NULL;
//
//     EditNode n1;
//     n1.buffer = strdup("Hello ");
//     n1.size = 6;
//     n1.capacity = 6;
//     n1.next = NULL;
//
//     EditBuffer b;
//     edit_buffer_init(&b);
//
//     edit_buffer_append_node(&b, &n1);
//     edit_buffer_append_node(&b, &n2);
//
//     assert(edit_buffer_size(&b) == 11);
//
//     char *s = edit_buffer_to_string(&b);
//     assert(!strcmp(s, "Hello World"));
//     free(s);
//
//     Location hello = location(LIST_BEGIN(&b), 0, 0);
//     assert(hello.index == 0);
//     assert(hello.offset == 0);
//     assert(hello.container == &n1);
//
//     Location world = find_next_word(&b, hello);
//     assert(world.index == 6);
//     assert(world.offset == 0);
//     assert(world.container == &n2);
//
//     Location end = find_next_word(&b, world);
//     assert(end.index == 6);
//     assert(end.offset == 4);
//     assert(end.container == &n2);
//
//     free(n1.buffer);
//     free(n2.buffer);
// }

void test_find_character()
{
}

int main()
{
    test0();
    // test1();
    // test2();
    // test_find_character();
    return 0;
}
