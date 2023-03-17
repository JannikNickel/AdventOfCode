#ifndef VEC2_H
#define VEC2_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	int32_t x;
	int32_t y;
} vec2;

vec2 vec2_new(int32_t x, int32_t y);

vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 v, int32_t s);

int32_t vec2_man_dst(vec2 a, vec2 b);
int32_t vec2_man_len(vec2 v);

size_t vec2_hash(vec2 v);
bool vec2_equals(vec2 a, vec2 b);

#endif