#include "solutions.h"
#include "common.h"

static vec2 find_station(const set* asteroids, int* visible);
static bool can_see(const set* asteroids, vec2 from, vec2 to);
static int32_t gcd(int32_t a, int32_t b);
static int cmp_angle(const void* a, const void* b);
static float dir_angle(float x, float y);
static set parse_input(const input* input);

static vec2 pt2_station;

result day10_part1(const input* input)
{
	set asteroids = parse_input(input);
	int most;
	find_station(&asteroids, &most);
	set_delete(&asteroids, NULL);
	return result_int(most);
}

result day10_part2(const input* input)
{
	vector list = vector_create(sizeof(vec2));
	set asteroids = parse_input(input);
	pt2_station = find_station(&asteroids, &(int) { 0 });

	set_iter it = set_iterator(&asteroids);
	vec2* asteroid;
	while((asteroid = set_iter_next(&it)) != NULL)
	{
		if(vec2_equals(*asteroid, pt2_station))
		{
			continue;
		}
		vector_push(&list, asteroid);
	}
	vector_sort(&list, cmp_angle);

	float angle = -1.0f;
	int removed = 0;
	vec2 last_removed;
	size_t index = 0;
	while(removed < 200)
	{
		vec2 p = *(vec2*)vector_at(&list, index);
		float a = dir_angle(p.x - pt2_station.x, p.y - pt2_station.y);
		if(a != angle)
		{
			size_t start = index;
			size_t end = start + 1;
			angle = a;
			while(end < list.size && dir_angle((*(vec2*)vector_at(&list, end)).x - pt2_station.x, (*(vec2*)vector_at(&list, end)).y - pt2_station.y) == angle)
			{
				end++;
			}
			size_t ci = start;
			int32_t c_dst = INT32_MAX;
			for(size_t i = start; i < end; i++)
			{
				int32_t dst = vec2_man_dst(*(vec2*)vector_at(&list, i), pt2_station);
				if(dst < c_dst)
				{
					c_dst = dst;
					ci = i;
				}
			}
			last_removed = *(vec2*)vector_at(&list, ci);
			vector_remove_at(&list, ci, NULL);
			if(ci == index)
			{
				index--;
			}
			removed++;
		}
		index = (index + 1) % list.size;
	}

	vector_delete(&list, NULL);
	set_delete(&asteroids, NULL);
	return result_int(last_removed.x * 100 + last_removed.y);
}

vec2 find_station(const set* asteroids, int* visible)
{
	*visible = 0;
	vec2* station = NULL;
	set_iter it = set_iterator(asteroids);
	vec2* asteroid;
	while((asteroid = set_iter_next(&it)) != NULL)
	{
		int c = 0;
		set_iter it2 = set_iterator(asteroids);
		vec2* other;
		while((other = set_iter_next(&it2)) != NULL)
		{
			if(asteroid == other)
			{
				continue;
			}
			c += can_see(asteroids, *asteroid, *other);
		}
		if(c > *visible)
		{
			*visible = c;
			station = asteroid;
		}
	}
	return *station;
}

bool can_see(const set* asteroids, vec2 from, vec2 to)
{
	vec2 dir = vec2_sub(to, from);
	int32_t step = gcd(abs(dir.x), abs(dir.y));
	vec2 dir_step = (vec2) { .x = dir.x / step, .y = dir.y / step };
	do
	{
		from = vec2_add(from, dir_step);
		if(!vec2_equals(from, to) && set_contains(asteroids, &from))
		{
			return false;
		}
	}
	while(!vec2_equals(from, to));
	return true;
}

int32_t gcd(int32_t a, int32_t b)
{
	if(b != 0)
	{
		return gcd(b, a % b);
	}
	return a;
}

int cmp_angle(const void* a, const void* b)
{
	vec2 _a = vec2_sub(*(vec2*)a, pt2_station);
	vec2 _b = vec2_sub(*(vec2*)b, pt2_station);
	float a_angle = dir_angle(_a.x, _a.y);
	float b_angle = dir_angle(_b.x, _b.y);
	return a_angle < b_angle ? -1 : (a_angle > b_angle ? 1 : 0);
}

float dir_angle(float x, float y)
{
	float a = atan2(y, x) * (180.0f / 3.14159265f);
	a = fmodf(a + 90.0f, 360.0f);
	if(a < 0.0f)
	{
		a += 360.0f;
	}
	return a;
}

set parse_input(const input* input)
{
	set set = set_create(sizeof(vec2), 128, 0.75f, NULL, NULL);
	for(size_t i = 0; i < input->lines.size; i++)
	{
		string* line = vector_at(&input->lines, i);
		for(size_t k = 0; k < line->length; k++)
		{
			if(string_at(*line, k) == '#')
			{
				set_insert(&set, &(vec2) { .x = k, .y = i });
			}
		}
	}
	return set;
}
