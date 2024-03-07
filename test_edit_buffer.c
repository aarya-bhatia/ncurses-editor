#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

void test0()
{
    EditBuffer b;
    edit_buffer_init(&b);
    edit_buffer_set_insert_position(&b, 0);
    edit_buffer_insert(&b, 'h');
    edit_buffer_insert(&b, 'i');
    char *s = edit_buffer_to_string(&b); 
	assert(!strcmp(s, "hi"));
    free(s);
    edit_buffer_free(&b);
}

/* void test0()
{
    EditBuffer *buffer = calloc(1, sizeof *buffer);
    for (int i = 0; i < 100; i++) {
        edit_buffer_insert(buffer, 0xa);
    }
    edit_buffer_free(buffer);
    free(buffer);
}

void test1()
{
    EditBuffer *buffer = calloc(1, sizeof *buffer);

    edit_buffer_insert(buffer, 'h');
    edit_buffer_insert(buffer, 'e');
    edit_buffer_insert(buffer, 'l');
    edit_buffer_insert(buffer, 'l');
    edit_buffer_insert(buffer, 'o');

    const char *res = edit_buffer_flush(buffer);
    puts(res);
    assert(strcmp(res, "hello") == 0);
    edit_buffer_free(buffer);
    free(buffer);
}

void test2()
{
    EditBuffer *buffer = calloc(1, sizeof *buffer);

    edit_buffer_insert(buffer, 'h');
    edit_buffer_insert(buffer, 'l');

    edit_buffer_set_position(buffer, 1);
    edit_buffer_insert(buffer, 'e');

    edit_buffer_set_position(buffer, 3);
    edit_buffer_insert(buffer, 'l');
    edit_buffer_insert(buffer, 'o');

    const char *res = edit_buffer_flush(buffer);
    puts(res);
    assert(strcmp(res, "hello") == 0);
    edit_buffer_free(buffer);
    free(buffer);
} */

int main()
{
    test0();
    return 0;
}
