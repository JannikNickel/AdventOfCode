#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

static set run_robot(string code_str, int64_t initial, size_t* painted);
static vec2 rotate(vec2 v, int dir);

result day11_part1(const input* input)
{
	size_t res;
	run_robot(input->raw, 0, &res);
	return result_uint(res);
}

result day11_part2(const input* input)
{
	set tiles = run_robot(input->raw, 1, &(int64_t) { 0 });

	vec2 min = (vec2) { .x = INT32_MAX, .y = INT32_MAX };
	vec2 max = (vec2) { .x = INT32_MIN, .y = INT32_MIN };
	set_iter it = set_iterator(&tiles);
	vec2* tile;
	while((tile = set_iter_next(&it)) != NULL)
	{
		min.x = min(min.x, tile->x);
		min.y = min(min.y, tile->y);
		max.x = max(max.x, tile->x);
		max.y = max(max.y, tile->y);
	}
	vec2 off = vec2_mul(min, -1);
	size_t lines = abs(max.y - min.y) + 1;
	size_t width = abs(max.x - min.x) + 2;
	vector l = vector_create(sizeof(string));
	for(size_t i = 0; i < lines; i++)
	{
		string empty = string_whitespace(width);
		*string_at_ref(empty, width - 1) = '\n';
		vector_push(&l, &empty);
	}

	it = set_iterator(&tiles);
	while((tile = set_iter_next(&it)) != NULL)
	{
		vec2 p = vec2_add(*tile, off);
		*string_at_ref(*(string*)vector_at(&l, p.y), p.x) = '@';
	}
	char* res = malloc(sizeof(char) * (lines * width + 1));
	res[lines * width] = '\0';
	for(size_t i = 0; i < lines; i++)
	{
		memcpy(&res[i * width], (*(string*)vector_at(&l, i)).data, sizeof(char) * width);
	}

	vector_delete(&l, string_dealloc);
	set_delete(&tiles, NULL);
	return result_heap_string(res);
}

set run_robot(string code_str, int64_t initial, size_t* painted)
{
	vector code = intcode_parse(code_str);
	set tiles = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL);
	set touched = set_create(sizeof(vec2), 2048, 0.75f, NULL, NULL);
	vector in = vector_from(sizeof(int64_t), &initial, 1);
	vector out = vector_create(sizeof(int64_t));

	size_t inst_ptr = 0;
	size_t rel_base = 0;
	vec2 pos = (vec2) { .x = 0.0f, .y = 0.0f };
	vec2 dir = (vec2) { .x = 0.0f, .y = -1.0f };
	intcode_run_result result;
	while((result = intcode_continue(&code, &in, &out, inst_ptr, rel_base, false, false)).type != IC_END)
	{
		inst_ptr = result.inst_ptr;
		rel_base = result.rel_base;
		int64_t color = *(int64_t*)vector_first(&out);
		int64_t turn = *(int64_t*)vector_last(&out);
		if(color)
		{
			set_insert(&tiles, &pos);
		}
		else
		{
			set_remove(&tiles, &pos, NULL);
		}
		set_insert(&touched, &pos);
		dir = rotate(dir, turn);
		pos = vec2_add(pos, dir);

		int64_t next_in = set_contains(&tiles, &pos);
		vector_push(&in, &next_in);
		vector_clear(&out, NULL);
	}
	*painted = touched.size;

	set_delete(&touched, NULL);
	vector_delete(&code, NULL);
	vector_delete(&in, NULL);
	vector_delete(&out, NULL);
	return tiles;
}

vec2 rotate(vec2 v, int dir)
{
	return dir == 1 ? vec2_new(-v.y, v.x) : vec2_new(v.y, -v.x);
}
