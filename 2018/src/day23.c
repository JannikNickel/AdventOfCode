#include "solutions.h"
#include "common.h"

typedef struct
{
	int64_t x;
	int64_t y;
	int64_t z;
} vec3;

typedef struct
{
	vec3 min;
	vec3 max;
} box3;

typedef struct
{
	vec3 pos;
	int64_t r;
} nanobot;

typedef struct
{
	int64_t count;
	int64_t dst;
	int64_t volume;
	box3 bounds;
} region;

static int64_t find_best_point_dst(const vector* nanobots);
static int64_t possible_count(const vector* nanobots, box3 box);
static size_t split_box(box3 box, box3 splits[8]);
static int64_t box_point_dst(box3 box, vec3 p);
static int64_t box_volume(box3 box);
static int64_t man_dst(vec3 a, vec3 b);
static int region_compare(const void* a, const void* b);
static vector parse_input(const input* input);

result day23_part1(const input* input)
{
	vector nanobots = parse_input(input);
	nanobot* strongest = vector_at(&nanobots, 0);
	for(size_t i = 1; i < nanobots.size; i++)
	{
		nanobot* bot = vector_at(&nanobots, i);
		if(bot->r > strongest->r)
		{
			strongest = bot;
		}
	}

	size_t in_range = 0;
	for(size_t i = 0; i < nanobots.size; i++)
	{
		nanobot* bot = vector_at(&nanobots, i);
		in_range += man_dst(strongest->pos, bot->pos) <= strongest->r;
	}

	vector_delete(&nanobots, NULL);
	return result_int(in_range);
}

result day23_part2(const input* input)
{
	vector nanobots = parse_input(input);
	int64_t best_distance = find_best_point_dst(&nanobots);
	vector_delete(&nanobots, NULL);
	return result_int(best_distance);
}

int64_t find_best_point_dst(const vector* nanobots)
{
	vec3 min_bound = { INT64_MAX, INT64_MAX, INT64_MAX };
	vec3 max_bound = { INT64_MIN, INT64_MIN, INT64_MIN };
	for(size_t i = 0; i < nanobots->size; i++)
	{
		const nanobot* bot = vector_at_c(nanobots, i);
		min_bound.x = min_i64(min_bound.x, bot->pos.x - bot->r);
		min_bound.y = min_i64(min_bound.y, bot->pos.y - bot->r);
		min_bound.z = min_i64(min_bound.z, bot->pos.z - bot->r);
		max_bound.x = max_i64(max_bound.x, bot->pos.x + bot->r);
		max_bound.y = max_i64(max_bound.y, bot->pos.y + bot->r);
		max_bound.z = max_i64(max_bound.z, bot->pos.z + bot->r);
	}

	heap open = heap_create(sizeof(region), region_compare, NULL);
	box3 start_box = { .min = min_bound, .max = max_bound };
	region start_reg =
	{
		.count = possible_count(nanobots, start_box),
		.dst = box_point_dst(start_box, (vec3){ 0, 0, 0 }),
		.volume = box_volume(start_box),
		.bounds = start_box
	};
	heap_push(&open, &start_reg);

	vec3 res = { 0, 0, 0 };
	while(open.size > 0)
	{
		region curr = *(const region*)heap_peek(&open);
		heap_pop(&open);

		if(curr.volume == 1)
		{
			res = curr.bounds.min;
			break;
		}

		box3 splits[8];
		size_t split_count = split_box(curr.bounds, splits);
		for(size_t i = 0; i < split_count; i++)
		{
			box3 split = splits[i];
			region reg =
			{
				.count = possible_count(nanobots, split),
				.dst = box_point_dst(split, (vec3){ 0, 0, 0 }),
				.volume = box_volume(split),
				.bounds = split
			};
			heap_push(&open, &reg);
		}
	}

	heap_delete(&open);
	return man_dst(res, (vec3){ 0, 0, 0 });
}

int64_t possible_count(const vector* nanobots, box3 box)
{
	int64_t count = 0;
	for(size_t i = 0; i < nanobots->size; i++)
	{
		const nanobot* bot = vector_at_c(nanobots, i);
		count += box_point_dst(box, bot->pos) <= bot->r;
	}
	return count;
}

size_t split_box(box3 box, box3 out[8])
{
	int64_t xm = box.min.x + (box.max.x - box.min.x) / 2;
	int64_t ym = box.min.y + (box.max.y - box.min.y) / 2;
	int64_t zm = box.min.z + (box.max.z - box.min.z) / 2;
	box3 splits[8] =
	{
		{ .min = { box.min.x, box.min.y, box.min.z }, .max = { xm, ym, zm } },
		{ .min = { xm + 1, box.min.y, box.min.z }, .max = { box.max.x, ym, zm } },
		{ .min = { box.min.x, ym + 1, box.min.z }, .max = { xm, box.max.y, zm } },
		{ .min = { xm + 1, ym + 1, box.min.z }, .max = { box.max.x, box.max.y, zm } },
		{ .min = { box.min.x, box.min.y, zm + 1 }, .max = { xm, ym, box.max.z } },
		{ .min = { xm + 1, box.min.y, zm + 1 }, .max = { box.max.x, ym, box.max.z } },
		{ .min = { box.min.x, ym + 1, zm + 1 }, .max = { xm, box.max.y, box.max.z } },
		{ .min = { xm + 1, ym + 1, zm + 1 }, .max = { box.max.x, box.max.y, box.max.z } },
	};
	size_t result = 0;
	for(int i = 0; i < 8; i++)
	{
		if(splits[i].min.x <= splits[i].max.x && splits[i].min.y <= splits[i].max.y && splits[i].min.z <= splits[i].max.z)
		{
			out[result++] = splits[i];
		}
	}
	return result;
}

int64_t box_point_dst(box3 box, vec3 p)
{
	int64_t dx = p.x < box.min.x || p.x > box.max.x ? min_i64(llabs(p.x - box.min.x), llabs(p.x - box.max.x)) : 0;
	int64_t dy = p.y < box.min.y || p.y > box.max.y ? min_i64(llabs(p.y - box.min.y), llabs(p.y - box.max.y)) : 0;
	int64_t dz = p.z < box.min.z || p.z > box.max.z ? min_i64(llabs(p.z - box.min.z), llabs(p.z - box.max.z)) : 0;
	return dx + dy + dz;
}

int64_t box_volume(box3 box)
{
	return (box.max.x - box.min.x + 1) * (box.max.y - box.min.y + 1) * (box.max.z - box.min.z + 1);
}

int64_t man_dst(vec3 a, vec3 b)
{
	return llabs(a.x - b.x) + llabs(a.y - b.y) + llabs(a.z - b.z);
}

static int region_compare(const void* a, const void* b)
{
	const region* r1 = a;
	const region* r2 = b;
	if(r1->count != r2->count)
	{
		return r1->count > r2->count ? -1 : 1;
	}
	if(r1->dst != r2->dst)
	{
		return r1->dst < r2->dst ? -1 : 1;
	}
	if(r1->volume != r2->volume)
	{
		return r1->volume < r2->volume ? -1 : 1;
	}
	return 0;
}

vector parse_input(const input* input)
{
	vector nanobots = vector_create(sizeof(nanobot));
	for(size_t i = 0; i < input->line_count; i++)
	{
		nanobot bot;
		sscanf(input->lines_c[i].data, "pos=<" SFMT_INT64_T "," SFMT_INT64_T "," SFMT_INT64_T ">, r=" SFMT_INT64_T, &bot.pos.x, &bot.pos.y, &bot.pos.z, &bot.r);
		vector_push(&nanobots, &bot);
	}
	return nanobots;
}
