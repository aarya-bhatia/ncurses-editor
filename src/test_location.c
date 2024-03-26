#include "edit_buffer.h"
#include "location.h"
#include <assert.h>
#include <string.h>

void test_one_node()
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

void test_many_node()
{
    EditNode *n1 = edit_node_new1("he");
    EditNode *n2 = edit_node_new1("ll");
    EditNode *n3 = edit_node_new1("o ");
    EditNode *n4 = edit_node_new1("worl");
    EditNode *n5 = edit_node_new1("d!");

    EditBuffer b;
    edit_buffer_init(&b);
    edit_buffer_append_node(&b, n1);
    edit_buffer_append_node(&b, n2);
    edit_buffer_append_node(&b, n3);
    edit_buffer_append_node(&b, n4);
    edit_buffer_append_node(&b, n5);

    assert(edit_buffer_size(&b) == 12);
    char *s = edit_buffer_to_string(&b);
    assert(!strcmp(s, "hello world!"));

    Location l = (Location){n1, n1->buffer};

    for (int i = 0; i < 12; i++) {
        assert(l.ptr[0] == s[i]);
        assert(location_ok(l));
        assert(location_to_index(l) == i);
        l = location_next_byte(l);
    }

    assert(!location_ok(l));

    free(s);
    edit_buffer_clear(&b);
}

void test_word_one_node()
{
    EditNode *n = edit_node_new1("hello world!");
    EditBuffer b;
    edit_buffer_init(&b);
    edit_buffer_append_node(&b, n);

    Location l = (Location){n, n->buffer};

    Location l1 = location_next_word(l);
    Location l2 = location_next_word(l1);
    Location l3 = location_next_word(l2);

    assert(l1.ptr[0] == 'w');
    assert(l2.ptr[0] == '!');
    assert(l3.ptr[0] == '!');

    Location l4 = location_prev_word(l2);
    Location l5 = location_prev_word(l4);
    Location l6 = location_prev_word(l5);

    assert(l4.ptr[0] == 'w');
    assert(l5.ptr[0] == 'h');
    assert(l6.ptr[0] == 'h');

    edit_buffer_clear(&b);
}

int main()
{
    test_one_node();
    test_many_node();
    test_word_one_node();

    return 0;
}
