#include "include/edit_buffer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test0()
{
    EditBuffer b;
    edit_buffer_init(&b);
    edit_buffer_set_insert_position(&b, 0);
    edit_buffer_insert(&b, 'i');
    edit_buffer_set_insert_position(&b, 0);
    edit_buffer_insert(&b, 'h');

    char *s = edit_buffer_to_string(&b);
    puts(s);
    assert(!strcmp(s, "hi"));
    free(s);

    edit_buffer_clear(&b);
}

void test1()
{
    EditBuffer buffer;
    edit_buffer_init(&buffer);

    for (int i = 0; i < 100; i++) {
        edit_buffer_insert(&buffer, 'a');
    }

    char *s = edit_buffer_to_string(&buffer);
    puts(s);
    free(s);

    edit_buffer_clear(&buffer);
}

void test2()
{
    EditBuffer buffer;
    edit_buffer_init(&buffer);

    edit_buffer_insert(&buffer, 'h');
    edit_buffer_insert(&buffer, 'e');
    edit_buffer_insert(&buffer, 'o');

    edit_buffer_set_insert_position(&buffer, 2);
    edit_buffer_insert(&buffer, 'l');
    edit_buffer_insert(&buffer, 'l');

    edit_buffer_set_insert_position(&buffer, 5);

    char *res = edit_buffer_to_string(&buffer);
    puts(res);
    assert(strcmp(res, "hello") == 0);
    free(res);

    edit_buffer_clear(&buffer);
}

void test3()
{
    EditBuffer buffer;
    edit_buffer_init(&buffer);

    edit_buffer_insert(&buffer, 'h');
    edit_buffer_insert(&buffer, 'l');

    edit_buffer_set_insert_position(&buffer, 1);
    edit_buffer_insert(&buffer, 'e');

    edit_buffer_set_insert_position(&buffer, 3);
    edit_buffer_insert(&buffer, 'l');
    edit_buffer_insert(&buffer, 'o');

    char *res = edit_buffer_to_string(&buffer);
    puts(res);
    assert(strcmp(res, "hello") == 0);
    free(res);

    edit_buffer_clear(&buffer);
}

int main()
{
    test0();
    test1();
    test2();
    test3();
    return 0;
}
