#pragma once

typedef struct Vec2 {
  int x;
  int y;
} Vec2;

Vec2 vec2_add(Vec2 u, Vec2 v);
Vec2 vec2_mul(Vec2 v, int scalar);
double vec2_dist(Vec2 v);
