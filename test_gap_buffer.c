#include "gap_buffer.h"
#include <assert.h>
#include <string.h>

void test_gap_buffer() {
  gapbuffer_t buf;
  memset(&buf, 0, sizeof buf);

  _gapbuffer_create_gap(&buf, 0, 10);
  _gapbuffer_create_gap(&buf, 0, 1);
  _gapbuffer_create_gap(&buf, 0, 20);
  _gapbuffer_create_gap(&buf, 0, 0);
  _gapbuffer_create_gap(&buf, 0, 20);

  assert(buf._capacity >= 20);
  assert(buf._size == 0);
  assert(buf._gap_enable);
  assert(buf._gap_used == 0);
  assert(buf._gap_size == 20);
  assert(buf._gap_start == 0);

  freegapbuffer(&buf);
}

int main() {
  test_gap_buffer();
  return 0;
}
