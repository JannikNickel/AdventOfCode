#include "solutions.h"
#include "common.h"

static const vec2 NEIGHBORS[8] =
{
	{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
	{1, 0}, {-1, 1}, {0, 1}, {1, 1}
};

typedef struct
{
	char* data;
	size_t width;
	size_t height;
} map;

typedef struct
{
	uint64_t hash;
	char* data;
	size_t size;
} map_state;

static size_t solve(const input* input, size_t iterations);
static void simulate(map* m);
static size_t count_neighbors(const map* m, size_t x, size_t y, char type);
static uint64_t map_hash(const map* m);
static bool map_state_pred(size_t index, const void* element, const void* ctx);
static void map_state_free(void* element);
static map parse_input(const input* input);

result day18_part1(const input* input)
{
	return result_int(solve(input, 10));
}

result day18_part2(const input* input)
{
	return result_int(solve(input, 1000000000));
}

size_t solve(const input* input, size_t iterations)
{
	map m = parse_input(input);
	vector seen = vector_create(sizeof(map_state));

	bool cycle_detected = false;
	for(size_t i = 0; i < iterations; i++)
	{
		simulate(&m);

		if(!cycle_detected)
		{
			map_state state = { .hash = map_hash(&m), .data = m.data, .size = m.width * m.height };
			size_t prev_idx = vector_index_of_pred(&seen, map_state_pred, &state);
			if(prev_idx != SIZE_MAX)
			{
				cycle_detected = true;
				size_t cycle_length = i - prev_idx;
				size_t remaining = (iterations - i) % cycle_length;
				i = iterations - remaining;
				continue;
			}

			state.data = malloc(state.size);
			memcpy(state.data, m.data, state.size);
			vector_push(&seen, &state);
		}
	}

	size_t trees = 0;
	size_t lumberyards = 0;
	for(size_t i = 0; i < m.width * m.height; i++)
	{
		trees += (m.data[i] == '|');
		lumberyards += (m.data[i] == '#');
	}

	vector_delete(&seen, map_state_free);
	free(m.data);
	return trees * lumberyards;
}

void simulate(map* m)
{
	char* next = malloc(m->width * m->height);
	for(size_t y = 0; y < m->height; y++)
	{
		for(size_t x = 0; x < m->width; x++)
		{
			char curr = m->data[y * m->width + x];
			if(curr == '.' && count_neighbors(m, x, y, '|') >= 3)
			{
				curr = '|';
			}
			else if(curr == '|' && count_neighbors(m, x, y, '#') >= 3)
			{
				curr = '#';
			}
			else if(curr == '#' && (count_neighbors(m, x, y, '#') == 0 || count_neighbors(m, x, y, '|') == 0))
			{
				curr = '.';
			}
			next[y * m->width + x] = curr;
		}
	}
	free(m->data);
	m->data = next;
}

size_t count_neighbors(const map* m, size_t x, size_t y, char type)
{
	size_t count = 0;
	for(size_t i = 0; i < 8; i++)
	{
		size_t nx = x + NEIGHBORS[i].x;
		size_t ny = y + NEIGHBORS[i].y;
		count += (nx < m->width && ny < m->height && m->data[ny * m->width + nx] == type);
	}
	return count;
}

uint64_t map_hash(const map* m)
{
	uint64_t hash = 14695981039346656037ULL;
	for(size_t i = 0; i < m->width * m->height; i++)
	{
		hash ^= (uint8_t)m->data[i];
		hash *= 1099511628211ULL;
	}
	return hash;
}

bool map_state_pred(size_t index, const void* element, const void* ctx)
{
	const map_state* state = element;
	const map_state* target = ctx;
	return state->hash == target->hash && memcmp(state->data, target->data, state->size) == 0;
}

void map_state_free(void* element)
{
	map_state* state = element;
	free(state->data);
}

map parse_input(const input* input)
{
	map result;
	result.width = input->lines_c[0].length;
	result.height = input->line_count;
	result.data = malloc(result.width * result.height);
	for(size_t i = 0; i < input->line_count; i++)
	{
		memcpy(result.data + i * result.width, input->lines_c[i].data, result.width);
	}
	return result;
}
