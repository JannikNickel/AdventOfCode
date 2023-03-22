#ifndef vec3_H
#define vec3_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	int32_t x;
	int32_t y;
	int32_t z;
} vec3;

vec3 vec3_new(int32_t x, int32_t y, int32_t z);
vec3 vec3_zero();

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 v, int32_t s);

int32_t vec3_man_dst(vec3 a, vec3 b);
int32_t vec3_man_len(vec3 v);

size_t vec3_hash(vec3 v);
bool vec3_equals(vec3 a, vec3 b);

#endif