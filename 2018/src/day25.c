#include "solutions.h"
#include "common.h"

typedef struct
{
	int x;
	int y;
	int z;
	int w;
} vec4;

static size_t count_sets(const vec4* points, size_t point_count);
static bool union_sets(size_t* parent, size_t* size, size_t a, size_t b);
static size_t find_set(size_t* parent, size_t a);
static int man_dst(vec4 a, vec4 b);
static size_t parse_input(const input* input, vec4** points);

result day25_part1(const input* input)
{
	vec4* points;
	size_t point_count = parse_input(input, &points);
	size_t set_count = count_sets(points, point_count);
	free(points);	
	return result_int(set_count);
}

result day25_part2(const input* input)
{
	//Day 25 only has 1 puzzle
	return result_none();
}

size_t count_sets(const vec4* points, size_t point_count)
{
	size_t* parent = malloc(sizeof(size_t) * point_count);
	size_t* size = malloc(sizeof(size_t) * point_count);
	for(size_t i = 0; i < point_count; i++)
	{
		parent[i] = i;
		size[i] = 1;
	}

	size_t set_count = point_count;
	for(size_t i = 0; i < point_count; i++)
	{
		for(size_t k = i + 1; k < point_count; k++)
		{
			if(man_dst(points[i], points[k]) <= 3)
			{
				if(union_sets(parent, size, i, k))
				{
					set_count--;
				}
			}
		}
	}

	free(parent);
	free(size);
	return set_count;
}

bool union_sets(size_t* parent, size_t* size, size_t a, size_t b)
{
	size_t root_a = find_set(parent, a);
	size_t root_b = find_set(parent, b);

	if(root_a == root_b)
	{
		return false;
	}

	if(size[root_a] < size[root_b])
	{
		size_t tmp = root_b;
		root_b = root_a;
		root_a = tmp;
	}
	parent[root_b] = root_a;
	size[root_a] += size[root_b];
	return true;
}

size_t find_set(size_t* parent, size_t i)
{
	while(parent[i] != i)
	{
		parent[i] = parent[parent[i]];
		i = parent[i];
	}
	return i;
}

int man_dst(vec4 a, vec4 b)
{
	return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z) + abs(a.w - b.w);
}

size_t parse_input(const input* input, vec4** points)
{
	*points = malloc(sizeof(vec4) * input->line_count);
	for(size_t i = 0; i < input->line_count; i++)
	{
		vec4* point = &(*points)[i];
		sscanf(input->lines_c[i].data, "%d,%d,%d,%d", &point->x, &point->y, &point->z, &point->w);
	}
	return input->line_count;
}
