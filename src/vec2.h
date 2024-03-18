#pragma once
#include <stdbool.h>

typedef struct Vec2 {
  int x;
  int y;
} Vec2;

bool vec2_equal(Vec2 u, Vec2 v);
double vec2_dist(Vec2 v);
Vec2 vec2_add(Vec2 u, Vec2 v);
Vec2 vec2_mul(Vec2 v, int scalar);
Vec2 vec2_sub(Vec2 u, Vec2 v);
