#include "solutions.h"
#include "common.h"

typedef enum
{
	EMPTY,
	BLOCKED,
	OUTER_PORTAL,
	INNER_PORTAL
} tile_type;

typedef struct tile
{
	tile_type type;
	vec3 pos;
	int depth;
	struct tile* portal;
} tile;

typedef struct
{
	tile** tiles;
	int width;
	int height;
} map;

typedef struct
{
	char id[2];
	vec2 p0;
	vec2 p1;
} portal;

static const vec2 DIRS[4] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

static int32_t solve(const input* input, bool dimensions);
static int32_t bfs(map map, vec2 from, vec2 to, bool dimensions);
static tile get_tile(map map, vec2 pos);
static size_t portal_hash(const portal* element);
static bool portal_equals(const portal* a, const portal* b);
static void map_delete(map* map);
static map parse_input(const input* input, vec2* start, vec2* target);
static void parse_identifier(const input* input, char id[2], int x, int y);

result day20_part1(const input* input)
{	
	return result_int(solve(input, false));
}

result day20_part2(const input* input)
{
	return result_int(solve(input, true));
}

int32_t solve(const input* input, bool dimensions)
{
	vec2 start, target;
	map map = parse_input(input, &start, &target);
	int32_t res = bfs(map, start, target, dimensions);
	map_delete(&map);
	return res;
}

int32_t bfs(map map, vec2 from, vec2 to, bool dimensions)
{
	vec3 target = vec3_new(to.x, to.y, 0);
	vector open = vector_from(sizeof(tile), &(tile) { .pos = vec3_new(from.x, from.y, 0), .type = EMPTY, .depth = 0 }, 1);
	set closed = set_create(sizeof(vec3), map.width * map.height * 2 * (dimensions ? 1024 : 1), 0.75f, NULL, NULL);
	int32_t res = -1;
	while(open.size != 0)
	{
		tile v = *(tile*)vector_first(&open);
		vector_remove_first(&open, NULL);

		vec3 p = v.pos;
		if(vec3_equals(p, target))
		{
			res = v.depth;
			break;
		}
		if(!set_insert(&closed, &p))
		{
			continue;
		}

		if(v.portal != NULL && (v.type == INNER_PORTAL || p.z > 0))
		{
			tile next = *v.portal;
			next.depth = v.depth + 1;
			next.pos.z = v.pos.z;
			if(dimensions)
			{
				next.pos.z += (v.type == INNER_PORTAL ? 1 : -1);
			}
			vector_push(&open, &next);
		}
		for(int i = 0; i < 4; i++)
		{
			tile nt = get_tile(map, vec2_add(vec2_new(v.pos.x, v.pos.y), DIRS[i]));
			nt.depth = v.depth + 1;
			nt.pos.z = v.pos.z;
			if(nt.type != BLOCKED)
			{
				vector_push(&open, &nt);
			}
		}
	}
	return res;
}

tile get_tile(map map, vec2 pos)
{
	if(pos.x < 0 || pos.y < 0 || pos.x >= map.width || pos.y >= map.height)
	{
		return (tile) { .type = BLOCKED, .portal = NULL };
	}
	return map.tiles[pos.y][pos.x];
}

size_t portal_hash(const portal* element)
{
	size_t h = 277;
	h = h * 47 + element->id[0];
	h = h * 47 + element->id[1];
	return h;
}

bool portal_equals(const portal* a, const portal* b)
{
	return a->id[0] == b->id[0] && a->id[1] == b->id[1];
}

void map_delete(map* map)
{
	for(int i = 0; i < map->height; i++)
	{
		free(map->tiles[i]);
	}
	free(map->tiles);
}

map parse_input(const input* input, vec2* start, vec2* target)
{
	int width = string_find_char(input->raw, '\n', 0) - 4;
	int height = input->lines.size - 4;
	vec2 inner_min = vec2_new(INT32_MAX, INT32_MAX);
	vec2 inner_max = vec2_new(INT32_MIN, INT32_MIN);
	tile** data = malloc(sizeof(tile*) * height);
	for(int i = 0; i < height; i++)
	{
		data[i] = malloc(sizeof(tile) * width);

		for(int k = 0; k < width; k++)
		{
			char c = string_at(*(string*)vector_at(&input->lines, i + 2), k + 2);
			if(c != '#' && c != '.')
			{
				inner_min.x = min(inner_min.x, k);
				inner_min.y = min(inner_min.y, i);
				inner_max.x = max(inner_max.x, k);
				inner_max.y = max(inner_max.y, i);
			}
		}
	}

	set portals = set_create(sizeof(portal), 32, 0.75f, portal_hash, portal_equals);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			bool is_border = y == 0 || x == 0 || y == height - 1 || x == width - 1;
			bool inner_border = (x >= inner_min.x - 1 && x <= inner_max.x + 1 && y >= inner_min.y - 1 && y <= inner_max.y + 1);
			is_border |= inner_border;
			char c = string_at(*(string*)vector_at(&input->lines, y + 2), x + 2);
			is_border &= c == '.';
			data[y][x] = (tile) { .type = (is_border ? (inner_border ? INNER_PORTAL : OUTER_PORTAL) : (c == '#' || c == ' ' ? BLOCKED : EMPTY)), .pos = vec3_new(x, y, 0), .portal = NULL };

			if(is_border)
			{
				char id[2];
				parse_identifier(input, id, x, y);
				portal p = (portal) { .id = { id[0], id[1] }, .p0 = vec2_new(x, y) };
				if(set_insert(&portals, &p))
				{
					continue;
				}
				portal* p_ptr = set_at(&portals, &p);
				p_ptr->p1 = vec2_new(x, y);
			}
		}
	}

	set_iter it = set_iterator(&portals);
	portal* p;
	while((p = set_iter_next(&it)) != NULL)
	{
		if((p->p0.x == 0 && p->p0.y == 0) || (p->p1.x == 0 && p->p1.y == 0))
		{
			continue;
		}
		tile* t0 = &data[p->p0.y][p->p0.x];
		tile* t1 = &data[p->p1.y][p->p1.x];
		t0->portal = t1;
		t1->portal = t0;
	}

	*start = ((portal*)set_at(&portals, &(portal) {.id = "AA" }))->p0;
	*target = ((portal*)set_at(&portals, &(portal) {.id = "ZZ" }))->p0;
	set_delete(&portals, NULL);
	return (map) { data, width, height };
}

void parse_identifier(const input* input, char id[2], int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		vec2 p = vec2_new(x + DIRS[i].x, y + DIRS[i].y);
		char c = string_at(*(string*)vector_at(&input->lines, p.y + 2), p.x + 2);
		if(isalpha(c))
		{
			char c2 = string_at(*(string*)vector_at(&input->lines, p.y + DIRS[i].y + 2), p.x + DIRS[i].x + 2);
			if(DIRS[i].x > 0 || DIRS[i].y > 0)
			{
				id[0] = c;
				id[1] = c2;
				break;
			}
			id[0] = c2;
			id[1] = c;
			break;
		}
	}
}
