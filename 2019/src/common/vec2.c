#include "vec2.h"
#include <math.h>

vec2 vec2_new(int32_t x, int32_t y)
{
	return (vec2) { .x = x, .y = y };
}

vec2 vec2_add(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x + b.x, .y = a.y + b.y };
}

vec2 vec2_sub(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x - b.x, .y = a.y - b.y };
}

vec2 vec2_mul(vec2 v, int32_t s)
{
	return (vec2) { .x = v.x * s, .y = v.y * s };
}

int32_t vec2_man_dst(vec2 a, vec2 b)
{
	return vec2_man_len(vec2_sub(a, b));
}

int32_t vec2_man_len(vec2 v)
{
	return abs(v.x) + abs(v.y);
}

size_t vec2_hash(vec2 v)
{
	size_t h = 277;
	h = h * 47 + v.x;
	h = h * 47 + v.y;
	return h;
}

bool vec2_equals(vec2 a, vec2 b)
{
	return a.x == b.x && a.y == b.y;
}
