#include "vec2.h"
#include <assert.h>

int main(void) {
  Vec2 v = {1, 3};
  v = vec2_add(v, (Vec2){1, 0});
  v = vec2_add(v, (Vec2){0, -1});
  v = vec2_mul(v, 4);
  v = vec2_add(v, (Vec2){-7, -8});

  assert(vec2_dist(v) == 1);
}
