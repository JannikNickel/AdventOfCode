#include "solutions.h"
#include "common.h"

// Sorted by read order
static const vec2 DIRS[] = 
{
	{.x = 0, .y = -1 },
	{.x = -1, .y = 0 },
	{.x = 1, .y = 0 },
	{.x = 0, .y = 1 }
};

typedef struct
{
	char* data;
	size_t width;
	size_t height;
} map;

typedef struct
{
	char type;
	vec2 pos;
	int hp;
	int attack_power;
} unit;

static bool simulate(const map* m, vector* units, bool stop_on_elf_kill);
static vec2 calc_move(const map* m, vector* units, unit* u);
static unit* find_adjacent_enemy(const map* m, vector* units, const unit* u);
static long bfs_distance(const map* m, const vector* units, vec2 start, vec2 target, long** dist_map);
static bool is_blocked(const map* m, const vector* units, vec2 pos);
static long count_units(const vector* units, char type);
static long sum_hp(const vector* units);
static int unit_read_order(const void* a, const void* b);
static int vec2_read_order(const void* a, const void* b);
static map parse_input(const input* input, vector* units);

result day15_part1(const input* input)
{
	vector units;
	map m = parse_input(input, &units);

	long round = 0;
	while(simulate(&m, &units, false))
	{
		round++;
	}
	long result = sum_hp(&units) * round;

	free(m.data);
	vector_delete(&units, NULL);
	return result_int(result);
}

result day15_part2(const input* input)
{
	vector units;
	map m = parse_input(input, &units);
	long initial_elves = count_units(&units, 'E');

	long result = 0;
	for(int i = 4;; i++)
	{
		vector units_copy = vector_clone(&units, NULL);
		for(size_t k = 0; k < units_copy.size; k++)
		{
			unit* u = vector_at(&units_copy, k);
			if(u->type == 'E')
			{
				u->attack_power = i;
			}
		}

		long round = 0;
		while(simulate(&m, &units_copy, true))
		{
			round++;
		}

		bool success = count_units(&units_copy, 'E') == initial_elves;
		result = sum_hp(&units_copy) * round;

		vector_delete(&units_copy, NULL);
		if(success)
		{
			break;
		}
	}

	free(m.data);
	vector_delete(&units, NULL);
	return result_int(result);
}

bool simulate(const map* m, vector* units, bool stop_on_elf_kill)
{
	vector_sort(units, unit_read_order);
	for(size_t i = 0; i < units->size; i++)
	{
		unit* u = vector_at(units, i);
		if(count_units(units, u->type == 'E' ? 'G' : 'E') == 0)
		{
			return false;
		}

		vec2 new_pos = calc_move(m, units, u);
		u->pos = new_pos;

		unit* target = find_adjacent_enemy(m, units, u);
		if(target != NULL)
		{
			target->hp -= u->attack_power;
			if(target->hp <= 0)
			{
				bool target_is_elf = target->type == 'E';
				size_t idx = vector_index_of(units, target);
				vector_remove_at(units, idx, NULL);
				if(idx < i)
				{
					i--;
				}
				if(stop_on_elf_kill && target_is_elf)
				{
					return false;
				}
			}
		}
	}
	return true;
}

vec2 calc_move(const map* m, vector* units, unit* u)
{
	if(find_adjacent_enemy(m, units, u))
	{
		return u->pos;
	}

	long best_dst = LONG_MAX;
	vec2 best_target = { 0, 0 };

	long* dist_map = NULL;
	bfs_distance(m, units, u->pos, (vec2){ -1, -1 }, &dist_map);
	for(size_t i = 0; i < units->size; i++)
	{
		const unit* other = vector_at_c(units, i);
		if(other->type == u->type)
		{
			continue;
		}

		vec2 pos = other->pos;
		for(size_t k = 0; k < 4; k++)
		{
			vec2 target = vec2_add(pos, DIRS[k]);
			if(is_blocked(m, units, target))
			{
				continue;
			}

			long dst = dist_map[target.y * m->width + target.x];
			if(dst < 0)
			{
				continue;
			}

			if(dst < best_dst || (dst == best_dst && vec2_read_order(&target, &best_target) < 0))
			{
				best_dst = dst;
				best_target = target;
			}
		}
	}

	vec2 result = u->pos;
	if(best_dst != LONG_MAX)
	{
		for(size_t i = 0; i < 4; i++)
		{
			vec2 from = vec2_add(u->pos, DIRS[i]);
			if(is_blocked(m, units, from))
			{
				continue;
			}

			long dst = bfs_distance(m, units, from, best_target, NULL);
			if(dst == best_dst - 1)
			{
				result = from;
				break;
			}
		}
	}

	free(dist_map);
	return result;
}

unit* find_adjacent_enemy(const map* m, vector* units, const unit* u)
{
	unit* result = NULL;
	for(size_t i = 0; i < units->size; i++)
	{
		unit* other = vector_at(units, i);
		if(other->type != u->type)
		{
			if(vec2_man_dst(other->pos, u->pos) == 1)
			{
				if(result == NULL || other->hp < result->hp || (other->hp == result->hp && unit_read_order(other, result) < 0))
				{
					result = other;
				}
			}
		}
	}
	return result;
}

long bfs_distance(const map* m, const vector* units, vec2 start, vec2 target, long** dist_map)
{
	long* dist = malloc(m->width * m->height * sizeof(long));
	memset(dist, -1, m->width * m->height * sizeof(long));

	vector queue = vector_create(sizeof(vec2));
	dist[start.y * m->width + start.x] = 0;
	vector_push(&queue, &start);

	long result = -1;
	size_t queue_head = 0;
	while(queue_head < queue.size)
	{
		vec2 curr = *(vec2*)vector_at(&queue, queue_head++);

		if(curr.x == target.x && curr.y == target.y)
		{
			result = dist[curr.y * m->width + curr.x];
			break;
		}

		for(size_t i = 0; i < 4; i++)
		{
			vec2 next = vec2_add(curr, DIRS[i]);
			if(!is_blocked(m, units, next) && dist[next.y * m->width + next.x] == -1)
			{
				dist[next.y * m->width + next.x] = dist[curr.y * m->width + curr.x] + 1;
				vector_push(&queue, &next);
			}
		}
	}

	vector_delete(&queue, NULL);
	if(dist_map != NULL)
	{
		*dist_map = dist;
	}
	else
	{
		free(dist);
	}
	return result;
}

bool is_blocked(const map* m, const vector* units, vec2 pos)
{
	if(pos.x < 0 || pos.y < 0 || pos.x >= m->width || pos.y >= m->height)
	{
		return true;
	}
	if(m->data[pos.y * m->width + pos.x] != '.')
	{
		return true;
	}
	for(size_t i = 0; i < units->size; i++)
	{
		const unit* u = vector_at_c(units, i);
		if(u->pos.x == pos.x && u->pos.y == pos.y)
		{
			return true;
		}
	}
	return false;
}

long count_units(const vector* units, char type)
{
	long count = 0;
	for(size_t i = 0; i < units->size; i++)
	{
		const unit* u = vector_at_c(units, i);
		count += u->type == type;
	}
	return count;
}

long sum_hp(const vector* units)
{
	long sum = 0;
	for(size_t i = 0; i < units->size; i++)
	{
		const unit* u = vector_at_c(units, i);
		sum += u->hp;
	}
	return sum;
}

int unit_read_order(const void* a, const void* b)
{
	const unit* ca = (const unit*)a;
	const unit* cb = (const unit*)b;
	if(ca->pos.y == cb->pos.y)
	{
		return ca->pos.x - cb->pos.x;
	}
	return ca->pos.y - cb->pos.y;
}

int vec2_read_order(const void* a, const void* b)
{
	const vec2* ca = (const vec2*)a;
	const vec2* cb = (const vec2*)b;
	if(ca->y == cb->y)
	{
		return ca->x - cb->x;
	}
	return ca->y - cb->y;
}

map parse_input(const input* input, vector* units)
{
	map m = { 0 };
	m.width = input->lines_c[0].length;
	m.height = input->line_count;
	m.data = malloc(m.width * m.height);
	*units = vector_create(sizeof(unit));

	for(size_t y = 0; y < m.height; y++)
	{
		const string* line = &input->lines_c[y];
		for(size_t x = 0; x < m.width; x++)
		{
			char c = line->data[x];
			if(c == 'G' || c == 'E')
			{
				unit u = { .type = c, .pos = { .x = (int32_t)x, .y = (int32_t)y }, .hp = 200, .attack_power = 3 };
				vector_push(units, &u);
				c = '.';
			}
			m.data[y * m.width + x] = c;
		}
	}
	return m;
}
