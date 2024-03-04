#include "edit_buffer.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_open_gap() {
  EditBuffer buf;
  memset(&buf, 0, sizeof buf);

  edit_buffer_open_gap(&buf, 0, 10);
  edit_buffer_open_gap(&buf, 0, 1);
  edit_buffer_open_gap(&buf, 0, 20);
  edit_buffer_open_gap(&buf, 0, 0);
  edit_buffer_open_gap(&buf, 0, 20);

  assert(buf.capacity >= 20);
  assert(buf.size == 20);
  assert(buf.gap_enable);
  assert(buf.gap_used == 0);
  assert(buf.gap_size == 20);
  assert(buf.gap_start == 0);

  edit_buffer_free(&buf);
}

void test_insert_simple() {
  EditBuffer buf;
  memset(&buf, 0, sizeof buf);

  unsigned char bytes[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  edit_buffer_open_gap(&buf, 0, sizeof bytes);

  for (int i = 0; i < sizeof bytes; i++) {
    edit_buffer_insert(&buf, bytes[i]);
  }

  edit_buffer_close_gap(&buf);

  assert(buf.size == sizeof bytes);
  assert(memcmp(buf.buffer, bytes, sizeof bytes) == 0);
  edit_buffer_free(&buf);
}

void test_insert_undo() {
  EditBuffer buf;
  memset(&buf, 0, sizeof buf);

  unsigned char bytes[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  edit_buffer_open_gap(&buf, 0, sizeof bytes);

  for (int i = 0; i < sizeof bytes; i++) {
    edit_buffer_insert(&buf, bytes[i]);
  }

  edit_buffer_gap_clear(&buf);
  edit_buffer_close_gap(&buf);

  assert(buf.size == 0);
  edit_buffer_free(&buf);
}

void test_insert_extend_gap() {
  EditBuffer buf;
  memset(&buf, 0, sizeof buf);

  edit_buffer_insert(&buf, 0x00);

  edit_buffer_open_gap(&buf, 1, 1);
  edit_buffer_insert(&buf, 0x01);
  edit_buffer_insert(&buf, 0x02);
  edit_buffer_insert(&buf, 0x03);
  edit_buffer_close_gap(&buf);

  edit_buffer_insert(&buf, 0x04);

  assert(buf.size == 5);
  assert(memcmp(buf.buffer, "\x00\x01\x02\x03\x04", 5) == 0);

  edit_buffer_free(&buf);
}

void test_gap_after_append() {
  EditBuffer buf;
  memset(&buf, 0, sizeof buf);

  edit_buffer_insert(&buf, 0x02);
  edit_buffer_open_gap(&buf, 0, 1);
  edit_buffer_insert(&buf, 0x01);
  edit_buffer_open_gap(&buf, 2, 1);
  edit_buffer_insert(&buf, 0x03);
  edit_buffer_close_gap(&buf);

  assert(buf.size == 3);
  assert(memcmp(buf.buffer, "\x01\x02\x03", 3) == 0);

  edit_buffer_free(&buf);
}

void test_backspace_inside_gap() {
  EditBuffer b;
  memset(&b, 0, sizeof b);
  edit_buffer_insert(&b, 'h');
  edit_buffer_insert(&b, 'o');

  edit_buffer_open_gap(&b, 1, 1);
  edit_buffer_insert(&b, 'e');
  edit_buffer_insert(&b, 'l');
  edit_buffer_insert(&b, 'n');
  edit_buffer_backspace(&b);
  edit_buffer_insert(&b, 'l');

  edit_buffer_close_gap(&b);

  assert(b.size == 5);
  assert(memcmp(b.buffer, "hello", 5) == 0);
  edit_buffer_free(&b);
}

void test_backspace_outside_gap() {
  EditBuffer b;
  memset(&b, 0, sizeof b);
  edit_buffer_insert(&b, 'h');
  edit_buffer_insert(&b, 'e');
  edit_buffer_insert(&b, 'n');
  edit_buffer_backspace(&b);
  edit_buffer_insert(&b, 'l');
  edit_buffer_insert(&b, 'x');
  edit_buffer_backspace(&b);
  edit_buffer_insert(&b, 'l');
  edit_buffer_insert(&b, 'o');

  assert(b.size == 5);
  assert(memcmp(b.buffer, "hello", 5) == 0);
  edit_buffer_free(&b);
}

void test_print_string() {
  EditBuffer b;
  memset(&b, 0, sizeof b);

  const char *str = "hello\n";

  for (int i = 0; i < strlen(str); i++) {
    edit_buffer_insert(&b, str[i]);
  }

  edit_buffer_print_string(&b, printf);
  edit_buffer_free(&b);
}

int main() {
  test_open_gap();
  test_insert_simple();
  test_insert_undo();
  test_insert_extend_gap();
  test_gap_after_append();
  test_backspace_inside_gap();
  test_backspace_outside_gap();
  test_print_string();
  return 0;
}
