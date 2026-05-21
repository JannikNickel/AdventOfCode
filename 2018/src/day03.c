#include "solutions.h"
#include "common.h"

typedef struct claim
{
	int32_t id;
	vec2 pos;
	vec2 size;
} claim;

static uint32_t* build_fabric(const vector* claims, vec2 max_cell, size_t* fabric_size);
static vector parse_input(const input* input, vec2* max_cell);

result day03_part1(const input* input)
{
	vec2 max_cell;
	vector claims = parse_input(input, &max_cell);
	size_t fabric_size;
	uint32_t* fabric = build_fabric(&claims, max_cell, &fabric_size);

	uint32_t overlap_count = 0;
	for(size_t i = 0; i < fabric_size; i++)
	{
		overlap_count += fabric[i] > 1;
	}

	free(fabric);
	vector_delete(&claims, NULL);
	return result_int(overlap_count);
}

result day03_part2(const input* input)
{
	vec2 max_cell;
	vector claims = parse_input(input, &max_cell);
	size_t fabric_size;
	uint32_t* fabric = build_fabric(&claims, max_cell, &fabric_size);

	uint32_t target_id = 0;
	for(size_t i = 0; i < claims.size; i++)
	{
		const claim* c = vector_at_c(&claims, i);
		bool overlaps = false;
		for(int32_t y = 0; y < c->size.y; y++)
		{
			for(int32_t x = 0; x < c->size.x; x++)
			{
				overlaps |= fabric[((c->pos.y + y) * max_cell.x) + (c->pos.x + x)] > 1;
			}
		}
		if(!overlaps)
		{
			target_id = c->id;
			break;
		}
	}

	free(fabric);
	vector_delete(&claims, NULL);
	return result_int(target_id);
}

uint32_t* build_fabric(const vector* claims, vec2 max_cell, size_t* fabric_size)
{
	*fabric_size = max_cell.x * max_cell.y;
	uint32_t* fabric = calloc(*fabric_size, sizeof(uint32_t));
	for(size_t i = 0; i < claims->size; i++)
	{
		const claim* c = vector_at_c(claims, i);
		for(int32_t y = 0; y < c->size.y; y++)
		{
			for(int32_t x = 0; x < c->size.x; x++)
			{
				fabric[((c->pos.y + y) * max_cell.x) + (c->pos.x + x)] += 1;
			}
		}
	}
	return fabric;
}

vector parse_input(const input* input, vec2* max_cell)
{
	*max_cell = (vec2) { 0, 0 };
	vector result = vector_create(sizeof(claim));
	for(size_t i = 0; i < input->line_count; i++)
	{
		const char* line = input->lines_c[i].data;
		const char* idStart = line + 1;
		const char* idEnd = strchr(line, '@');
		const char* posMid = strchr(line, ',');
		const char* posEnd = strchr(line, ':');
		const char* sizeMid = strchr(line, 'x');
		if(idEnd && posMid && posEnd && sizeMid)
		{
			int32_t id = atoi(idStart);
			int32_t posX = atoi(idEnd + 1);
			int32_t posY = atoi(posMid + 1);
			int32_t sizeX = atoi(posEnd + 1);
			int32_t sizeY = atoi(sizeMid + 1);
			claim c = { .id = id, .pos = { posX, posY }, .size = { sizeX, sizeY } };
			vector_push(&result, &c);

			max_cell->x = max(max_cell->x, posX + sizeX);
			max_cell->y = max(max_cell->y, posY + sizeY);
		}
	}
	return result;
}
