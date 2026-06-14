#include "solutions.h"
#include "common.h"

static const vec2 DIRS[] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

typedef enum
{
	TORCH,
	CLIMBING_GEAR,
	NEITHER,
	TOOL_COUNT
} tool;

typedef enum
{
	ROCKY,
	WET,
	NARROW
} region;

typedef struct
{
	size_t x;
	size_t y;
	tool tool;
	uint32_t time;
} state;

static uint32_t* create_grid(size_t width, size_t height, size_t target_x, size_t target_y, uint32_t depth);
static uint32_t calc_path(uint32_t* grid, size_t width, size_t height, size_t target_x, size_t target_y);
static bool tool_allowed(region r, tool t);
static size_t hash_state(const void* s);
static bool state_equals(const void* a, const void* b);
static int state_compare(const void* a, const void* b);
static uint32_t parse_input(const input* input, size_t* target_x, size_t* target_y);

result day22_part1(const input* input)
{
	size_t target_x, target_y;
	uint32_t depth = parse_input(input, &target_x, &target_y);
	uint32_t* grid = create_grid(target_x + 1, target_y + 1, target_x, target_y, depth);
	uint32_t risk_level = 0;
	for(size_t i = 0; i < (target_x + 1) * (target_y + 1); i++)
	{
		risk_level += grid[i];
	}
	free(grid);
	return result_int(risk_level);
}

result day22_part2(const input* input)
{
	size_t target_x, target_y;
	uint32_t depth = parse_input(input, &target_x, &target_y);
	size_t width = target_x + 100;
	size_t height = target_y + 100;
	uint32_t* grid = create_grid(width, height, target_x, target_y, depth);
	uint32_t path_length = calc_path(grid, width, height, target_x, target_y);
	free(grid);
	return result_int(path_length);
}

uint32_t* create_grid(size_t width, size_t height, size_t target_x, size_t target_y, uint32_t depth)
{
	uint32_t* grid = malloc(width * height * sizeof(uint32_t));
	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			uint32_t geo_index;
			if((x == 0 && y == 0) || (x == target_x && y == target_y))
			{
				geo_index = 0;
			}
			else if(y == 0)
			{
				geo_index = x * 16807;
			}
			else if(x == 0)
			{
				geo_index = y * 48271;
			}
			else
			{
				geo_index = grid[y * width + x - 1] * grid[(y - 1) * width + x];
			}
			grid[y * width + x] = (geo_index + depth) % 20183;
		}
	}

	for(size_t y = 0; y < height; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			grid[y * width + x] %= 3;
		}
	}
	return grid;
}

uint32_t calc_path(uint32_t* grid, size_t width, size_t height, size_t target_x, size_t target_y)
{
	heap open = heap_create(sizeof(state), state_compare, NULL);
	heap_push(&open, &(state){ .x = 0, .y = 0, .tool = TORCH, .time = 0 });
	set closed = set_create(sizeof(state), 0, 0.75, hash_state, state_equals);

	uint32_t res = UINT32_MAX;
	while(open.size > 0)
	{
		state curr = *(state*)heap_peek(&open);
		heap_pop(&open);

		if(!set_insert(&closed, &curr))
		{
			continue;
		}

		if(curr.x == target_x && curr.y == target_y && curr.tool == TORCH)
		{
			res = curr.time;
			break;
		}

		uint32_t curr_region = grid[curr.y * width + curr.x];
		for(size_t i = 0; i < 4; i++)
		{
			size_t nx = curr.x + DIRS[i].x;
			size_t ny = curr.y + DIRS[i].y;
			if(nx < width && ny < height)
			{
				uint32_t next_region = grid[ny * width + nx];
				for(tool t = 0; t < TOOL_COUNT; t++)
				{
					if(tool_allowed(curr_region, t) && tool_allowed(next_region, t))
					{
						heap_push(&open, &(state){.x = nx, .y = ny, .tool = t, .time = curr.time + 1 + (t != curr.tool ? 7 : 0) });
					}
				}
			}
		}

		if(curr.x == target_x && curr.y == target_y && curr.tool != TORCH)
		{
			heap_push(&open, &(state){.x = curr.x, .y = curr.y, .tool = TORCH, .time = curr.time + 7 });
		}
	}

	set_delete(&closed, NULL);
	heap_delete(&open);
	return res;
}

bool tool_allowed(region r, tool t)
{
	switch(r)
	{
		case ROCKY:
			return t == CLIMBING_GEAR || t == TORCH;
		case WET:
			return t == CLIMBING_GEAR || t == NEITHER;
		case NARROW:
			return t == TORCH || t == NEITHER;
	}
	return false;
}

size_t hash_state(const void* s)
{
	const state* st = s;
	size_t h = 15127993;
	h = h * 5737 + st->x;
	h = h * 5737 + st->y;
	h = h * 5737 + st->tool;
	return h;
}

bool state_equals(const void* a, const void* b)
{
	const state* sa = a;
	const state* sb = b;
	return sa->x == sb->x && sa->y == sb->y && sa->tool == sb->tool;
}

int state_compare(const void* a, const void* b)
{
	const state* sa = a;
	const state* sb = b;
	return (int)sa->time - (int)sb->time;
}

uint32_t parse_input(const input* input, size_t* target_x, size_t* target_y)
{
	uint32_t depth = 0;
	sscanf(input->lines_c[0].data, "depth: " SFMT_UINT32_T, &depth);
	sscanf(input->lines_c[1].data, "target: %zu,%zu", target_x, target_y);
	return depth;
}
