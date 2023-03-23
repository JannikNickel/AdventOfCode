#include "solutions.h"
#include "common.h"

typedef struct
{
	vec2 key;
	int32_t wire;
	int32_t steps;
} cell;

typedef struct
{
	char dir;
	int steps;
} instruction;

static int32_t find_closest(const input* wires, bool distance);
static size_t cell_hash(const cell* element);
static bool cell_equals(const cell* a, const cell* b);
static vector parse_input(const input* input);

result day03_part1(const input* input)
{
	int32_t closest = find_closest(input, true);
	return result_int(closest);
}

result day03_part2(const input* input)
{
	int32_t closest = find_closest(input, false);
	return result_int(closest);
}

int32_t find_closest(const input* input, bool distance)
{
	vector wires = parse_input(input);
	int32_t c_dst = INT32_MAX;
	int32_t c_steps = INT32_MAX;
	set set = set_create(sizeof(cell), 1024 * 512, 0.75f, cell_hash, cell_equals);
	for(size_t wire = 0; wire < wires.size; wire++)
	{
		vector_iter inst_it = vector_iterator(vector_at(&wires, wire));
		instruction* inst;
		vec2 pos = vec2_new(0, 0);
		uint32_t steps = 0;
		while((inst = vector_iter_next(&inst_it)) != NULL)
		{
			vec2 dir;
			switch(inst->dir)
			{
				case 'L': dir = vec2_new(-1, 0); break;
				case 'U': dir = vec2_new(0, 1); break;
				case 'R': dir = vec2_new(1, 0); break;
				case 'D': dir = vec2_new(0, -1); break;
			}
			for(int i = 0; i < inst->steps; i++)
			{
				steps++;
				pos = vec2_add(pos, dir);
				cell key = (cell) { .key = pos, .wire = wire, .steps = steps };
				if(set_contains(&set, &key))
				{
					cell* c = set_at(&set, &key);
					if(c->wire != wire)
					{
						c_dst = min(c_dst, vec2_man_len(pos));
						c_steps = min(c_steps, c->steps + steps);
					}
				}
				else
				{
					set_insert(&set, &key);
				}
			}
		}
	}
	vector_delete(&wires, vector_dealloc);
	return distance ? c_dst : c_steps;
}

size_t cell_hash(const cell* element)
{
	return vec2_hash(element->key);
}

bool cell_equals(const cell* a, const cell* b)
{
	return vec2_equals(a->key, b->key);
}

vector parse_input(const input* input)
{
	vector wires = vector_create(sizeof(vector));
	for(size_t i = 0; i < input->lines.size; i++)
	{
		vector instructions = vector_create(sizeof(instruction));
		vector parts = string_split_all_cstr(*(string*)vector_at(&input->lines, i), ",");
		vector_iter it = vector_iterator(&parts);
		string* s;
		while((s = vector_iter_next(&it)) != NULL)
		{
			instruction i = (instruction) { .dir = s->data[0], .steps = atoi(&s->data[1]) };
			vector_push(&instructions, &i);
		}
		vector_push(&wires, &instructions);
		vector_delete(&parts, string_dealloc);
	}
	return wires;
}
