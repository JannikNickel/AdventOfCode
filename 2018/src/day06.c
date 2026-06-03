#include "solutions.h"
#include "common.h"

static size_t find_largest_finite_area(const vector* points, vec2 min, vec2 max);
static vector parse_input(const input* input, vec2* min, vec2* max);

result day06_part1(const input* input)
{
	vec2 min, max;
	vector points = parse_input(input, &min, &max);
	size_t largest_area = find_largest_finite_area(&points, min, max);
	vector_delete(&points, NULL);
	return result_int(largest_area);
}

result day06_part2(const input* input)
{
	const int32_t LIMIT = 10000;

	vec2 min, max;
	vector points = parse_input(input, &min, &max);

	// Expanding one cell above bounds horizontally or vertically adds points.size to total distance
	int32_t padding = LIMIT / (int32_t)points.size;
	min.x = min.x - padding;
	min.y = min.y - padding;
	max.x = max.x + padding;
	max.y = max.y + padding;

	size_t region_size = 0;
	for(int32_t x = min.x; x <= max.x; x++)
	{
		for(int32_t y = min.y; y <= max.y; y++)
		{
			size_t total_dst = 0;
			for(size_t i = 0; i < points.size; i++)
			{
				const vec2* p = (const vec2*)vector_at_c(&points, i);
				total_dst += abs(p->x - x) + abs(p->y - y);
			}
			if(total_dst < LIMIT)
			{
				region_size++;
			}
		}
	}
	vector_delete(&points, NULL);
	return result_int(region_size);
}

size_t find_largest_finite_area(const vector* points, vec2 min, vec2 max)
{
	size_t* area_sizes = calloc(points->size, sizeof(size_t));
	for(int32_t y = min.y; y <= max.y; y++)
	{
		for(int32_t x = min.x; x <= max.x; x++)
		{
			size_t min_dst = SIZE_MAX;
			size_t closest_point = SIZE_MAX;
			for(size_t i = 0; i < points->size; i++)
			{
				const vec2* point = (const vec2*)vector_at_c(points, i);
				size_t dst = abs(point->x - x) + abs(point->y - y);
				if(dst < min_dst)
				{
					min_dst = dst;
					closest_point = i;
				}
				else if(dst == min_dst)
				{
					closest_point = SIZE_MAX;
				}
			}

			size_t point_index = closest_point;
			if(point_index != SIZE_MAX && area_sizes[point_index] != SIZE_MAX)
			{
				area_sizes[point_index]++;
				if(x == min.x || y == min.y || x == max.x || y == max.y)
				{
					area_sizes[point_index] = SIZE_MAX;
				}
			}
		}
	}

	size_t largest_area = 0;
	for(size_t i = 0; i < points->size; i++)
	{
		if(area_sizes[i] != SIZE_MAX && area_sizes[i] > largest_area)
		{
			largest_area = area_sizes[i];
		}
	}

	free(area_sizes);
	return largest_area;
}

vector parse_input(const input* input, vec2* _min, vec2* _max)
{
	vector points = vector_create(sizeof(vec2));
	_min->x = INT32_MAX;
	_min->y = INT32_MAX;
	_max->x = INT32_MIN;
	_max->y = INT32_MIN;
	for(size_t i = 0; i < input->line_count; i++)
	{
		vec2 point;
		sscanf(input->lines_c[i].data, SFMT_INT32_T "," SFMT_INT32_T, &point.x, &point.y);
		vector_push(&points, &point);
		_min->x = min(_min->x, point.x);
		_min->y = min(_min->y, point.y);
		_max->x = max(_max->x, point.x);
		_max->y = max(_max->y, point.y);
	}
	return points;
}
