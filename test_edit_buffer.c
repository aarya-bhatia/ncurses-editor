#include "edit_buffer.h"
#include <assert.h>
#include <string.h>

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
}

int main()
{
    test1();
    test2();
    return 0;
}
