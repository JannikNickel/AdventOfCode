#include "solutions.h"
#include "common.h"

#define MOONS 4

typedef struct
{
	vec3 pos;
	vec3 vel;
} moon;

static void simulate(moon moons[MOONS]);
static void gravity(moon moons[MOONS]);
static void velocity(moon moons[MOONS]);
static uint64_t lcm(uint64_t a, uint64_t b);
static uint64_t gcd(uint64_t a, uint64_t b);
static void parse_input(const input* input, moon moons[MOONS]);

result day12_part1(const input* input)
{
	moon moons[MOONS];
	parse_input(input, moons);
	for(size_t i = 0; i < 1000; i++)
	{
		simulate(moons);
	}
	uint32_t energy = 0;
	for(size_t i = 0; i < MOONS; i++)
	{
		energy += vec3_man_len(moons[i].pos) * vec3_man_len(moons[i].vel);
	}
	return result_uint(energy);
}

result day12_part2(const input* input)
{
	moon moons[MOONS];
	parse_input(input, moons);
	moon initial[MOONS];
	memcpy(initial, moons, sizeof(moons));

	uint64_t x_loop = 0;
	uint64_t y_loop = 0;
	uint64_t z_loop = 0;
	uint64_t c = 0;
	while(!x_loop || !y_loop || !z_loop)
	{
		simulate(moons);
		c++;

		bool x = true;
		bool y = true;
		bool z = true;
		for(size_t i = 0; i < MOONS; i++)
		{
			x &= moons[i].pos.x == initial[i].pos.x && moons[i].vel.x == initial[i].vel.x;
			y &= moons[i].pos.y == initial[i].pos.y && moons[i].vel.y == initial[i].vel.y;
			z &= moons[i].pos.z == initial[i].pos.z && moons[i].vel.z == initial[i].vel.z;
		}
		if(x && x_loop == 0)
		{
			x_loop = c;
		}
		if(y && y_loop == 0)
		{
			y_loop = c;
		}
		if(z && z_loop == 0)
		{
			z_loop = c;
		}
	}
	return result_uint(lcm(lcm(x_loop, y_loop), z_loop));
}

void simulate(moon moons[MOONS])
{
	gravity(moons);
	velocity(moons);
}

void gravity(moon moons[MOONS])
{
	for(size_t i = 0; i < MOONS; i++)
	{
		for(size_t k = 0; k < MOONS; k++)
		{
			if(i == k)
			{
				continue;
			}
			moon* a = &moons[i];
			moon b = moons[k];
			vec3 dir = vec3_sub(b.pos, a->pos);
			a->vel.x += (dir.x > 0) - (dir.x < 0);
			a->vel.y += (dir.y > 0) - (dir.y < 0);
			a->vel.z += (dir.z > 0) - (dir.z < 0);
		}
	}
}

void velocity(moon moons[MOONS])
{
	for(size_t i = 0; i < MOONS; i++)
	{
		moons[i].pos = vec3_add(moons[i].pos, moons[i].vel);
	}
}

uint64_t lcm(uint64_t a, uint64_t b)
{
	return a * b / gcd(a, b);
}

uint64_t gcd(uint64_t a, uint64_t b)
{
	if(b != 0)
	{
		return gcd(b, a % b);
	}
	return a;
}

void parse_input(const input* input, moon moons[MOONS])
{
	for(size_t i = 0; i < input->lines.size; i++)
	{
		string* line = vector_at(&input->lines, i);
		size_t x = string_find_char(*line, '=', 0) + 1;
		size_t y = string_find_char(*line, '=', x) + 1;
		size_t z = string_find_char(*line, '=', y) + 1;
		vec3 p = (vec3) { .x = strtol(&line->data[x], NULL, 10), .y = strtol(&line->data[y], NULL, 10), .z = strtol(&line->data[z], NULL, 10) };
		moons[i] = (moon) { .pos = p, .vel = vec3_zero() };
	}
}
