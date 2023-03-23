#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

typedef struct
{
	vec2 pos;
	uint64_t depth;
	vector code;
	size_t inst_ptr;
	size_t rel_base;
} state;

static const int64_t DIR_INP[] = { 1, 2, 3, 4 };
static const vec2 OFF[4] = { { 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 } };

static void discover_map(string code_str, set* map, vec2* oxy_pos, uint64_t* oxy_depth);
static state* state_new(vec2 pos, uint64_t depth, vector code, size_t inst_ptr, size_t rel_base);

result day15_part1(const input* input)
{
	uint64_t res;
	discover_map(input->raw, NULL, NULL, &res);
	return result_uint(res);
}

result day15_part2(const input* input)
{
	set map;
	vec2 origin;
	discover_map(input->raw, &map, &origin, NULL);

	set oxy = set_create(sizeof(vec2), map.size, 2.0f, NULL, NULL);
	set_insert(&oxy, &origin);
	uint64_t depth = 0;
	while(map.size > oxy.size)
	{
		vector to_add = vector_create(sizeof(vec2));
		set_iter it = set_iterator(&oxy);
		vec2* v;
		while((v = set_iter_next(&it)) != NULL)
		{
			for(size_t i = 0; i < 4; i++)
			{
				vec2 p = vec2_add(*v, OFF[i]);
				if(set_contains(&map, &p))
				{
					vector_push(&to_add, &p);
				}
			}
		}

		for(size_t i = 0; i < to_add.size; i++)
		{
			set_insert(&oxy, vector_at(&to_add, i));
		}
		vector_delete(&to_add, NULL);
		depth++;
	}

	set_delete(&map, NULL);
	return result_uint(depth);
}

void discover_map(string code_str, set* map, vec2* oxy_pos, uint64_t* oxy_depth)
{
	vector code = intcode_parse(code_str);
	set visited = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL);
	vector states = vector_create(sizeof(state*));
	state* initial = state_new(vec2_new(0, 0), 0, vector_clone(&code, NULL), 0, 0);
	vector_push(&states, &initial);

	while(states.size > 0)
	{
		state* s = *(state**)vector_first(&states);
		vector_remove_first(&states, NULL);

		if(set_contains(&visited, &s->pos))
		{
			continue;
		}
		set_insert(&visited, &s->pos);

		for(size_t i = 0; i < 4; i++)
		{
			int64_t dir = DIR_INP[i];
			vector code = vector_clone(&s->code, NULL);
			vector in = vector_from(sizeof(int64_t), &dir, 1);
			vector out = vector_create(sizeof(int64_t));

			intcode_run_result res = intcode_continue(&code, &in, &out, s->inst_ptr, s->rel_base, false, true);
			int64_t tile = *(int64_t*)vector_first(&out);
			vec2 p = vec2_add(s->pos, OFF[dir - 1]);

			if(tile == 1)
			{
				state* new_state = state_new(p, s->depth + 1, code, res.inst_ptr, res.rel_base);
				vector_push(&states, &new_state);
			}
			else if(tile == 2)
			{
				if(oxy_pos != NULL)
				{
					*oxy_pos = p;
				}
				if(oxy_depth != NULL)
				{
					*oxy_depth = s->depth + 1;
				}
			}
			vector_delete(&in, NULL);
			vector_delete(&out, NULL);
		}
		vector_delete(&s->code, NULL);
		free(s);
	}

	vector_delete(&code, NULL);
	vector_delete(&states, NULL);
	if(map != NULL)
	{
		*map = visited;
	}
	else
	{
		set_delete(&visited, NULL);
	}
}

state* state_new(vec2 pos, uint64_t depth, vector code, size_t inst_ptr, size_t rel_base)
{
	state* s = malloc(sizeof(state));
	s->pos = pos;
	s->depth = depth;
	s->code = code;
	s->inst_ptr = inst_ptr;
	s->rel_base = rel_base;
	return s;
}
