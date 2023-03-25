#include "solutions.h"
#include "common.h"

static const vec2 DIRS[4] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

typedef struct
{
	char** data;
	int width;
	int height;
} map;

typedef struct
{
	vec2 pos;
	uint32_t keys;
	uint32_t depth;
} state;

static int32_t best_path(map map, vec2 start, uint32_t keys);
static vec2 map_find(map map, char c);
static char map_get(map map, int x, int y);
static void map_delete(map* map);
static uint32_t find_keys(map map, vec2 min, vec2 max);
static size_t state_hash(const state* element);
static bool state_equals(const state* a, const state* b);
static uint32_t popcnt(uint32_t value);
static map parse_input(const input* input);

result day18_part1(const input* input)
{
	map map = parse_input(input);
	vec2 origin = map_find(map, '@');
	uint32_t best = best_path(map, origin, 0);
	map_delete(&map);
	return result_uint(best);
}

result day18_part2(const input* input)
{
	map map = parse_input(input);
	vec2 origin = map_find(map, '@');
	vec2 origins[4];
	int origin_index = 0;
	for(int i = -1; i <= 1; i++)
	{
		for(int k = -1; k <= 1; k++)
		{
			map.data[origin.y + k][origin.x + i] = (abs(i) && abs(k)) ? '@' : '#';
			if(abs(i) && abs(k))
			{
				origins[origin_index++] = vec2_new(origin.x + i, origin.y + k);
			}
		}
	}

	uint32_t inv_mask = (1 << popcnt(find_keys(map, vec2_new(0, 0), vec2_new(map.width, map.height)))) - 1;
	uint32_t keys[4] =
	{
		(find_keys(map, vec2_new(0, 0), origins[0]) & inv_mask) ^ inv_mask,
		(find_keys(map, vec2_new(0, origins[1].y), vec2_new(origins[1].x, map.height)) & inv_mask) ^ inv_mask,
		(find_keys(map, vec2_new(origins[2].x, 0), vec2_new(map.width, origins[2].y)) & inv_mask) ^ inv_mask,
		(find_keys(map, origins[3], vec2_new(map.width, map.height)) & inv_mask) ^ inv_mask
	};

	uint32_t best = 0;
	for(size_t i = 0; i < 4; i++)
	{
		best += best_path(map, origins[i], keys[i]);
	}

	map_delete(&map);
	return result_uint(best);
}

int32_t best_path(map map, vec2 start, uint32_t keys)
{
	uint32_t all_keys = find_keys(map, vec2_new(0, 0), vec2_new(map.width, map.height));
	vector open = vector_from(sizeof(state), &(state) { .pos = start, .keys = keys, .depth = 0 }, 1);
	vector_set_capacity(&open, map.width * map.height);
	set closed = set_create(sizeof(state), map.width * map.height * 26 * 256, 0.75f, state_hash, state_equals);
	uint32_t depth = -1;
	while(open.size != 0)
	{
		state curr = *(state*)vector_first(&open);
		vector_remove_first(&open, NULL);

		if(curr.keys == all_keys)
		{
			depth = curr.depth;
			break;
		}
		if(!set_insert(&closed, &curr))
		{
			continue;
		}

		for(int i = 0; i < 4; i++)
		{
			vec2 np = vec2_add(curr.pos, DIRS[i]);
			char c = map_get(map, np.x, np.y);
			if(c != '#' && (!isupper(c) || (curr.keys & (1 << (tolower(c) - 'a')))))
			{
				state next = (state) { .pos = np, .keys = (!islower(c) ? curr.keys : (curr.keys | (1 << (tolower(c) - 'a')))), .depth = curr.depth + 1 };
				vector_push(&open, &next);
			}
		}
	}
	vector_delete(&open, NULL);
	set_delete(&closed, NULL);
	return depth;
}

vec2 map_find(map map, char c)
{
	for(int i = 0; i < map.width; i++)
	{
		for(int k = 0; k < map.height; k++)
		{
			if(map_get(map, i, k) == c)
			{
				return vec2_new(i, k);
			}
		}
	}
}

char map_get(map map, int x, int y)
{
	if(x < 0 || y < 0 || x >= map.width || y >= map.height)
	{
		return '#';
	}
	return map.data[y][x];
}

void map_delete(map* map)
{
	for(int i = 0; i < map->height; i++)
	{
		free(map->data[i]);
	}
	free(map->data);
}

uint32_t find_keys(map map, vec2 min, vec2 max)
{
	uint32_t keys = 0;
	for(int i = min.x; i <= min(max.x, map.width - 1); i++)
	{
		for(int k = min.y; k <= min(max.y, map.height - 1); k++)
		{
			char c = map_get(map, i, k);
			if(isalpha(c) && islower(c))
			{
				keys |= (1 << (c - 'a'));
			}
		}
	}
	return keys;
}

size_t state_hash(const state* element)
{
	size_t h = 277;
	h = h * 47 + vec2_hash(element->pos);
	h = h * 47 + element->keys;
	return h;
}

bool state_equals(const state* a, const state* b)
{
	return vec2_equals(a->pos, b->pos) && a->keys == b->keys;
}

uint32_t popcnt(uint32_t value)
{
	uint32_t i = value;
	i -= ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	i = (i + (i >> 4)) & 0x0F0F0F0F;
	return (i * 0x01010101) >> 24;
}

map parse_input(const input* input)
{
	int width = string_find_char(input->raw, '\n', 0);
	int height = input->lines.size;
	char** data = malloc(sizeof(char*) * height);
	for(int i = 0; i < height; i++)
	{
		data[i] = malloc(sizeof(char) * width);
		memcpy(data[i], ((string*)vector_at(&input->lines, i))->data, sizeof(char) * width);
	}
	return (map) { data, width, height };
}
