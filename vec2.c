#include "vec2.h"
#include <math.h>

Vec2 vec2_add(Vec2 u, Vec2 v) { return (Vec2){u.x + v.x, u.y + v.y}; }

Vec2 vec2_mul(Vec2 u, int scalar) { return (Vec2){u.x * scalar, u.y * scalar}; }

double vec2_dist(Vec2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }
