#include "solutions.h"
#include "common.h"

static const int DIRS[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

static uint32_t neighbours_dim(uint32_t map, int x, int y, const uint32_t* lower, const uint32_t* upper);
static int64_t bio_div(uint32_t map);
static void set_bit(uint32_t* v, int index, bool bit);
static bool get_bit_2d(uint32_t v, int x, int y);
static bool get_bit(uint32_t v, int index);
static uint32_t parse_input(const input* input);

result day24_part1(const input* input)
{
	uint32_t map = parse_input(input);
	set seen = set_create(sizeof(uint32_t), 128, 0.75f, NULL, NULL);
	while(set_insert(&seen, &map))
	{
		uint32_t next = 0;
		for(int i = 0; i < 25; i++)
		{
			int neighbours = 0;
			for(int k = 0; k < 4; k++)
			{
				neighbours += get_bit_2d(map, i % 5 + DIRS[k][0], i / 5 + DIRS[k][1]);
			}
			set_bit(&next, i, get_bit(map, i) ? (neighbours == 1) : (neighbours == 1 || neighbours == 2));
		}
		map = next;
	}
	set_delete(&seen, NULL);
	return result_int(bio_div(map));
}

result day24_part2(const input* input)
{
	uint32_t map = parse_input(input);
	vector levels = vector_from(sizeof(uint32_t), &map, 1);

	for(int _ = 0; _ < 200; _++)
	{
		if(*(uint32_t*)vector_first(&levels) != 0)
		{
			vector_insert(&levels, 0, &(uint32_t) { 0 });
		}
		if(*(uint32_t*)vector_last(&levels) != 0)
		{
			vector_push(&levels, &(uint32_t) { 0 });
		}

		vector next_levels = vector_clone(&levels, NULL);
		for(size_t i = 0; i < levels.size; i++)
		{
			uint32_t* outer = i > 0 ? vector_at(&levels, i - 1) : NULL;
			uint32_t* level = vector_at(&levels, i);
			uint32_t* inner = i < levels.size - 1 ? vector_at(&levels, i + 1) : NULL;

			uint32_t next = 0;
			for(int i = 0; i < 25; i++)
			{
				if(i == 12)
				{
					continue;
				}
				int x = i % 5;
				int y = i / 5;
				int neighbours = neighbours_dim(*level, x, y, inner, outer);
				set_bit(&next, i, get_bit(*level, i) ? (neighbours == 1) : (neighbours == 1 || neighbours == 2));
			}
			vector_set(&next_levels, i, &next, NULL);
		}

		vector_delete(&levels, NULL);
		levels = next_levels;
	}

	uint32_t amount = 0;
	for(int i = 0; i < levels.size; i++)
	{
		uint32_t map = *(uint32_t*)vector_at(&levels, i);
		for(int i = 0; i < 25; i++)
		{
			amount += get_bit(map, i);
		}
	}
	vector_delete(&levels, NULL);
	return result_int(amount);
}

uint32_t neighbours_dim(uint32_t map, int x, int y, const uint32_t* inner, const uint32_t* outer)
{
	int idx = y * 5 + x;
	int neighbours = 0;
	for(int k = 0; k < 4; k++)
	{
		int nx = x + DIRS[k][0];
		int ny = y + DIRS[k][1];
		neighbours += get_bit_2d(map, nx, ny);

		if(outer != NULL)
		{
			if(nx == -1)
			{
				neighbours += get_bit(*outer, 11);
			}
			else if(nx == 5)
			{
				neighbours += get_bit(*outer, 13);
			}
			else if(ny == -1)
			{
				neighbours += get_bit(*outer, 7);
			}
			else if(ny == 5)
			{
				neighbours += get_bit(*outer, 17);
			}
		}

		if(inner != NULL && ny * 5 + nx == 12)
		{
			static const int LOWER_INDICES[4][5] =
			{
				{ 0, 5, 10, 15, 20 },
				{ 4, 9, 14, 19, 24 },
				{ 0, 1, 2, 3, 4 },
				{ 20, 21, 22, 23, 24 }
			};
			int* indices = &LOWER_INDICES[idx == 11 ? 0 : (idx == 13 ? 1 : (idx == 7 ? 2 : 3))];
			for(int i = 0; i < 5; i++)
			{
				neighbours += get_bit(*inner, indices[i]);
			}
		}
	}
	return neighbours;
}

int64_t bio_div(uint32_t map)
{
	int64_t bio = 0;
	int64_t rating = 1;
	for(int i = 0; i < 25; i++)
	{
		bio += get_bit(map, i) * rating;
		rating *= 2;
	}
	return bio;
}

void set_bit(uint32_t* v, int index, bool bit)
{
	*v = bit ? (*v | (1 << index)) : (*v & ~(1 << index));
}

bool get_bit(uint32_t v, int index)
{
	return (v & (1 << index));
}

bool get_bit_2d(uint32_t v, int x, int y)
{
	if(x < 0 || y < 0 || x > 4 || y > 4)
	{
		return false;
	}
	return get_bit(v, y * 5 + x);
}

uint32_t parse_input(const input* input)
{
	uint32_t v = 0;
	int index = 0;
	for(size_t i = 0; i < input->raw.length; i++)
	{
		char c = string_at(input->raw, i);
		set_bit(&v, index, c == '#');
		index += (c != '\n');
	}
	return v;
}
