#include "solutions.h"
#include "common.h"

typedef struct
{
	vec2 pos;
	char c;
} tile;

typedef struct
{
	vec2 pos;
	const char* str;
	const char* next_str;
} origin;

typedef struct
{
	vec2 pos;
	int32_t depth;
} bfs_node;

static set build_map(const input* input);
static void bfs_longest(const set* map, int32_t* max_depth, int32_t* rooms_1000);
static size_t find_paren_end(const char* str, vector* starts);
static vec2 move_pos(vec2 pos, char c, int32_t length);
static size_t tile_hash(const void* element);
static bool tile_equals(const void* a, const void* b);

result day20_part1(const input* input)
{
	set map = build_map(input);
	int32_t max_depth = 0;
	bfs_longest(&map, &max_depth, NULL);
	set_delete(&map, NULL);
	return result_int(max_depth);
}

result day20_part2(const input* input)
{
	set map = build_map(input);
	int32_t rooms_1000 = 0;
	bfs_longest(&map, NULL, &rooms_1000);
	set_delete(&map, NULL);
	return result_int(rooms_1000);
}

set build_map(const input* input)
{
	set map = set_create(sizeof(tile), input->raw.length, 0.75f, tile_hash, tile_equals);
	set_insert(&map, &(tile){ .pos = { 0, 0 }, .c = 'X' });

	vector open = vector_create(sizeof(origin));
	vector_push(&open, &(origin){ .pos = { 0, 0 }, .str = input->raw.data + 1, .next_str = NULL });
	set known_states = set_create(sizeof(origin), input->raw.length, 0.75f, NULL, NULL);

	size_t head = 0;
	while(head < open.size)
	{
		origin curr = *(origin*)vector_at(&open, head++);

		origin state = { .pos = curr.pos, .str = curr.str, .next_str = NULL };
		if(!set_insert(&known_states, &state))
		{
			continue;
		}

		const char* str = curr.str;
		size_t i = 0;
		while(str[i] != '$' && str[i] != '\0')
		{
			char dir = str[i];
			if(dir == 'N' || dir == 'E' || dir == 'S' || dir == 'W')
			{
				vec2 door_pos = move_pos(curr.pos, dir, 1);
				vec2 room_pos = move_pos(curr.pos, dir, 2);
				set_insert(&map, &(tile){ .pos = door_pos, .c = (dir == 'N' || dir == 'S') ? '-' : '|' });
				set_insert(&map, &(tile){ .pos = room_pos, .c = '.' });
				curr.pos = room_pos;
			}
			else if(dir == '(')
			{
				vector starts;
				size_t par_end = find_paren_end(str + i, &starts);
				if(par_end != SIZE_MAX)
				{
					const char* next_str = str + i + par_end + 1;
					for(size_t k = 0; k < starts.size; k++)
					{
						size_t* start = vector_at(&starts, k);
						vector_push(&open, &(origin){ .pos = curr.pos, .str = str + i + *start, .next_str = next_str });
					}
				}
				vector_delete(&starts, NULL);
				break;
			}
			else
			{
				if(curr.next_str != NULL)
				{
					vector_push(&open, &(origin){.pos = curr.pos, .str = curr.next_str, .next_str = NULL });
				}
				break;
			}
			i++;
		}
	}

	set_delete(&known_states, NULL);
	vector_delete(&open, NULL);
	return map;
}

void bfs_longest(const set* map, int32_t* max_depth, int32_t* rooms_1000)
{
	vector queue = vector_create(sizeof(bfs_node));
	set visited = set_create(sizeof(tile), map->size, 0.75f, tile_hash, tile_equals);

	vector_push(&queue, &(bfs_node){.pos = { 0, 0 }, .depth = 0 });
	set_insert(&visited, &(tile){.pos = { 0, 0 }, .c = '.' });

	size_t head = 0;
	while(head < queue.size)
	{
		bfs_node curr = *(bfs_node*)vector_at(&queue, head++);
		if(max_depth != NULL && curr.depth > *max_depth)
		{
			*max_depth = curr.depth;
		}
		if(rooms_1000 != NULL && curr.depth >= 1000)
		{
			*rooms_1000 += 1;
		}

		const char dirs[] = { 'N', 'E', 'S', 'W' };
		for(size_t i = 0; i < 4; i++)
		{
			const tile* door = set_at_c(map, &(tile){.pos = move_pos(curr.pos, dirs[i], 1) });
			const tile* room = set_at_c(map, &(tile){.pos = move_pos(curr.pos, dirs[i], 2) });
			if(door == NULL || room == NULL || room->c != '.')
			{
				continue;
			}

			if(set_insert(&visited, room))
			{
				vector_push(&queue, &(bfs_node){.pos = room->pos, .depth = curr.depth + 1 });
			}
		}
	}

	vector_delete(&queue, NULL);
	set_delete(&visited, NULL);
}

size_t find_paren_end(const char* str, vector* starts)
{
	*starts = vector_create(sizeof(size_t));

	int depth = 0;
	for(size_t i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '(')
		{
			depth++;
			if(depth == 1)
			{
				vector_push(starts, &(size_t){ i + 1 });
			}
		}
		else if(str[i] == '|' && depth == 1)
		{
			vector_push(starts, &(size_t){ i + 1 });
		}
		else if(str[i] == ')')
		{
			depth--;
			if(depth == 0)
			{
				return i;
			}
		}
	}
	return SIZE_MAX;
}

vec2 move_pos(vec2 pos, char c, int32_t length)
{
	switch(c)
	{
		case 'N':
			pos.y -= length;
			break;
		case 'E':
			pos.x += length;
			break;
		case 'S':
			pos.y += length;
			break;
		case 'W':
			pos.x -= length;
			break;
	}
	return pos;
}

size_t tile_hash(const void* element)
{
	const tile* t = element;
	return vec2_hash(t->pos);
}

bool tile_equals(const void* a, const void* b)
{
	const tile* t1 = a;
	const tile* t2 = b;
	return vec2_equals(t1->pos, t2->pos);
}
