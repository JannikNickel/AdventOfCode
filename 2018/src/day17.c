#include "solutions.h"
#include "common.h"

static const vec2 WATER_ORIGIN = { .x = 500, .y = 0 };

typedef struct
{
	set clay;
	set water;
	set flowing_water;
	size_t y_min;
	size_t y_max;
} map;

static void simulate_water(map* map, vec2 pos);
static bool scan_dir(const map* map, vec2 pos, int32_t dir, int32_t* end_pos);
static bool is_free(const map* map, vec2 pos);
static map parse_input(const input* input);

result day17_part1(const input* input)
{
	map m = parse_input(input);
	simulate_water(&m, WATER_ORIGIN);

	size_t water_count = m.water.size;
	set_iter iter = set_iterator(&m.flowing_water);
	vec2* pos;
	while((pos = set_iter_next(&iter)) != NULL)
	{
		water_count += pos->y >= m.y_min && pos->y <= m.y_max;
	}

	set_delete(&m.clay, NULL);
	set_delete(&m.water, NULL);
	set_delete(&m.flowing_water, NULL);
	return result_int(water_count);
}

result day17_part2(const input* input)
{
	map m = parse_input(input);

	simulate_water(&m, WATER_ORIGIN);
	size_t water_count = m.water.size;

	set_delete(&m.clay, NULL);
	set_delete(&m.water, NULL);
	set_delete(&m.flowing_water, NULL);
	return result_int(water_count);
}

void simulate_water(map* map, vec2 pos)
{
	vector stack = vector_create(sizeof(vec2));
	vector_push(&stack, &pos);

	while(stack.size > 0)
	{
		vec2 curr = *(vec2*)vector_at(&stack, stack.size - 1);
		vector_remove_last(&stack, NULL);

		while(curr.y <= map->y_max && is_free(map, vec2_add(curr, (vec2){ 0, 1 })))
		{
			set_insert(&map->flowing_water, &curr);
			curr.y++;
		}

		if(curr.y > map->y_max)
		{
			continue;
		}

		while(true)
		{
			int32_t left_end, right_end;
			bool left_blocked = scan_dir(map, curr, -1, &left_end);
			bool right_blocked = scan_dir(map, curr, 1, &right_end);
			if(left_blocked && right_blocked)
			{
				for(int32_t i = left_end; i <= right_end; i++)
				{
					set_remove(&map->flowing_water, &(vec2){.x = i, .y = curr.y }, NULL);
					set_insert(&map->water, &(vec2){.x = i, .y = curr.y });
				}
				curr.y--;
			}
			else
			{
				for(int32_t i = left_end; i <= right_end; i++)
				{
					set_insert(&map->flowing_water, &(vec2){.x = i, .y = curr.y });
				}
				if(!left_blocked)
				{
					vector_push(&stack, &(vec2){.x = left_end, .y = curr.y });
				}
				if(!right_blocked)
				{
					vector_push(&stack, &(vec2){.x = right_end, .y = curr.y });
				}
				break;
			}
		}
	}
	vector_delete(&stack, NULL);
}

bool scan_dir(const map* map, vec2 pos, int32_t dir, int32_t* end_pos)
{
	while(true)
	{
		if(is_free(map, vec2_add(pos, (vec2){ 0, 1 })))
		{
			*end_pos = pos.x;
			return false;
		}

		vec2 next_pos = vec2_add(pos, (vec2){ dir, 0 });
		if(set_contains(&map->clay, &next_pos))
		{
			*end_pos = pos.x;
			return true;
		}
		pos = next_pos;
	}
}

bool is_free(const map* map, vec2 pos)
{
	return !set_contains(&map->clay, &pos) && !set_contains(&map->water, &pos);
}

map parse_input(const input* input)
{
	map result =
	{
		.clay = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL),
		.water = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL),
		.flowing_water = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL),
		.y_min = SIZE_MAX,
		.y_max = 0,
	};

	for(size_t i = 0; i < input->line_count; i++)
	{
		const char* str = input->lines_c[i].data;
		int32_t l, r0, r1;
		if(str[0] == 'x')
		{
			sscanf(str, "x=" SFMT_INT32_T ", y=" SFMT_INT32_T ".." SFMT_INT32_T, &l, &r0, &r1);
			for(int32_t i = r0; i <= r1; i++)
			{
				set_insert(&result.clay, &(vec2){.x = l, .y = i });
				result.y_max = i > result.y_max ? i : result.y_max;
				result.y_min = i < result.y_min ? i : result.y_min;
			}
		}
		else
		{
			sscanf(str, "y=" SFMT_INT32_T ", x=" SFMT_INT32_T ".." SFMT_INT32_T, &l, &r0, &r1);
			for(int32_t i = r0; i <= r1; i++)
			{
				set_insert(&result.clay, &(vec2){.x = i, .y = l });
			}
			result.y_max = l > result.y_max ? l : result.y_max;
			result.y_min = l < result.y_min ? l : result.y_min;
		}
	}
	return result;
}
