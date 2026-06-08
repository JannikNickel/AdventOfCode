#include "solutions.h"
#include "common.h"

typedef struct
{
	char* data;
	size_t width;
	size_t height;
} map;

typedef struct
{
	vec2 pos;
	vec2 dir;
	int turn;
} cart;

static vec2 simulate_step(const map* map, vector* carts, bool ret_col);
static int cart_order(const void* a, const void* b);
static size_t find_cart_at_pos(const vector* carts, vec2 pos, size_t exclude_idx);
static map parse_input(const input* input, vector* carts);

result day13_part1(const input* input)
{
	vector carts = vector_create(sizeof(cart));
	map map = parse_input(input, &carts);

	vec2 collision = { -1, -1 };
	while(collision.x == -1 && collision.y == -1)
	{
		collision = simulate_step(&map, &carts, true);
	}

	free(map.data);
	vector_delete(&carts, NULL);
	return result_fmt(SFMT_INT32_T "," SFMT_INT32_T, collision.x, collision.y);
}

result day13_part2(const input* input)
{
	vector carts = vector_create(sizeof(cart));
	map map = parse_input(input, &carts);

	while(carts.size > 1)
	{
		simulate_step(&map, &carts, false);
	}
	vec2 result = ((cart*)vector_at(&carts, 0))->pos;

	free(map.data);
	vector_delete(&carts, NULL);
	return result_fmt(SFMT_INT32_T "," SFMT_INT32_T, result.x, result.y);
}

vec2 simulate_step(const map* map, vector* carts, bool ret_col)
{
	vector_sort(carts, cart_order);
	for(size_t i = 0; i < carts->size;)
	{
		cart* c = (cart*)vector_at(carts, i);
		c->pos.x += c->dir.x;
		c->pos.y += c->dir.y;

		char curr_track = map->data[c->pos.y * map->width + c->pos.x];
		switch(curr_track)
		{
			case '/':
			case '\\':
			{
				int32_t turn = curr_track == '/' ? -1 : 1;
				if(c->dir.x != 0)
				{
					c->dir = (vec2) { 0, c->dir.x * turn };
				}
				else
				{
					c->dir = (vec2) { c->dir.y * turn, 0 };
				}
				break;
			}
			case '+':
				switch(c->turn)
				{
					case 0:
						c->dir = (vec2){ c->dir.y, -c->dir.x };
						break;
					case 1:
						break;
					case 2:
						c->dir = (vec2){ -c->dir.y, c->dir.x };
						break;
				}
				c->turn = (c->turn + 1) % 3;
				break;
			default:
				break;
		}

		size_t collision_idx = find_cart_at_pos(carts, c->pos, i);
		if(collision_idx != SIZE_MAX)
		{
			if(ret_col)
			{
				return c->pos;
			}

			size_t low_idx = i < collision_idx ? i : collision_idx;
			size_t high_idx = i > collision_idx ? i : collision_idx;
			vector_remove_at(carts, high_idx, NULL);
			vector_remove_at(carts, low_idx, NULL);
			i = low_idx;
			continue;
		}

		i++;
	}
	return (vec2){ -1, -1 };
}

int cart_order(const void* a, const void* b)
{
	const cart* ca = (const cart*)a;
	const cart* cb = (const cart*)b;
	if(ca->pos.y == cb->pos.y)
	{
		return ca->pos.x - cb->pos.x;
	}
	return ca->pos.y - cb->pos.y;
}

size_t find_cart_at_pos(const vector* carts, vec2 pos, size_t exclude_idx)
{
	for(size_t i = 0; i < carts->size; i++)
	{
		if(i == exclude_idx)
		{
			continue;
		}

		const cart* c = (const cart*)vector_at_c(carts, i);
		if(c->pos.x == pos.x && c->pos.y == pos.y)
		{
			return i;
		}
	}
	return SIZE_MAX;
}

map parse_input(const input* input, vector* carts)
{
	map result = { 0 };
	result.width = input->lines_c[0].length;
	result.height = input->line_count;
	result.data = malloc(result.width * result.height);
	for(int32_t y = 0; y < result.height; y++)
	{
		for(int32_t x = 0; x < result.width; x++)
		{
			char c = input->lines_c[y].data[x];
			switch(c)
			{
				case '^':
				case 'v':
					vector_push(carts, &(cart){ .pos = { x, y }, .dir = { 0, c == '^' ? -1 : 1 }, .turn = 0 });
					c = '|';
					break;
				case '<':
				case '>':
					vector_push(carts, &(cart){ .pos = { x, y }, .dir = { c == '<' ? -1 : 1, 0 }, .turn = 0 });
					c = '-';
					break;
				default:
					break;
			}
			result.data[y * result.width + x] = c;
		}
	}
	return result;
}
