#include "vec3.h"
#include <math.h>

vec3 vec3_new(int32_t x, int32_t y, int32_t z)
{
	return (vec3) { .x = x, .y = y, .z = z };
}

vec3 vec3_zero()
{
	return (vec3) { .x = 0, .y = 0, .z = 0 };
}

vec3 vec3_add(vec3 a, vec3 b)
{
	return (vec3) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

vec3 vec3_sub(vec3 a, vec3 b)
{
	return (vec3) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

vec3 vec3_mul(vec3 v, int32_t s)
{
	return (vec3) { .x = v.x * s, .y = v.y * s, .z = v.z * s };
}

int32_t vec3_man_dst(vec3 a, vec3 b)
{
	return vec3_man_len(vec3_sub(a, b));
}

int32_t vec3_man_len(vec3 v)
{
	return abs(v.x) + abs(v.y) + abs(v.z);
}

size_t vec3_hash(vec3 v)
{
	size_t h = 277;
	h = h * 47 + v.x;
	h = h * 47 + v.y;
	h = h * 47 + v.z;
	return h;
}

bool vec3_equals(vec3 a, vec3 b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}
