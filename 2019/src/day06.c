#include "solutions.h"
#include "common.h"

typedef struct
{
	string id;
	vector* orbit;
} object;

static uint64_t count_orbit(const set* objects, const object* obj, uint32_t depth);
static bool find_path(const set* objects, const string* from, const string* to, vector* path, uint32_t depth);
static size_t object_hash(const object* element);
static bool object_equals(const object* a, const object* b);
static void object_dealloc(object* obj);
static set parse_input(const input* input);

result day06_part1(const input* input)
{
	set objects = parse_input(input);
	string root_id = string_from("COM");
	object* root = set_at(&objects, &(object) { .id = root_id });
	uint64_t res = count_orbit(&objects, root, 0);
	string_delete(&root_id);
	set_delete(&objects, object_dealloc);
	return result_uint(res);
}

result day06_part2(const input* input)
{
	set objects = parse_input(input);
	string root_id = string_from("COM");
	string t0_id = string_from("YOU");
	string t1_id = string_from("SAN");

	vector p0 = vector_create(sizeof(string));
	find_path(&objects, &root_id, &t0_id, &p0, 0);
	vector p1 = vector_create(sizeof(string));
	find_path(&objects, &root_id, &t1_id, &p1, 0);

	size_t len = min(p0.size, p1.size);
	string common_ancestor = root_id;
	for(size_t i = 0; i < len; i++)
	{
		if(!string_equals(*(string*)vector_at(&p0, i), *(string*)vector_at(&p1, i)))
		{
			common_ancestor = *(string*)vector_at(&p0, i - 1);
			break;
		}
	}

	size_t transfers = 0;
	vector_clear(&p0, NULL);
	vector_clear(&p1, NULL);
	find_path(&objects, &common_ancestor, &t0_id, &p0, 0);
	find_path(&objects, &common_ancestor, &t1_id, &p1, 0);
	transfers = p0.size + p1.size - 2 - 2;

	string_delete(&root_id);
	string_delete(&t0_id);
	string_delete(&t1_id);
	vector_delete(&p0, NULL);
	vector_delete(&p1, NULL);
	set_delete(&objects, object_dealloc);
	return result_uint(transfers);
}

uint64_t count_orbit(const set* objects, const object* obj, uint32_t depth)
{
	uint64_t sum = depth;
	if(obj != NULL && obj->orbit != NULL)
	{
		vector_iter it = vector_iterator(obj->orbit);
		string* id;
		while((id = vector_iter_next(&it)) != NULL)
		{
			object* obj = set_at(objects, &(object) { .id = *id });
			sum += count_orbit(objects, obj, depth + 1);
		}
	}
	return sum;
}

bool find_path(const set* objects, const string* from, const string* to, vector* path, uint32_t depth)
{
	vector_push(path, from);

	if(string_equals(*from, *to))
	{
		return true;
	}

	object* from_obj = set_at(objects, &(object) { .id = *from });
	if(from_obj != NULL && from_obj->orbit != NULL)
	{
		vector_iter it = vector_iterator(from_obj->orbit);
		string* id;
		while((id = vector_iter_next(&it)) != NULL)
		{
			object* obj = set_at(objects, &(object) {.id = *id });
			if(!find_path(objects, id, to, path, depth + 1))
			{
				while(path->size > depth + 1)
				{
					vector_remove_at(path, path->size - 1, NULL);
				}
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

size_t object_hash(const object* element)
{
	return string_hash(element->id);
}

bool object_equals(const object* a, const object* b)
{
	return string_equals(a->id, b->id);
}

void object_dealloc(object* obj)
{
	if(obj != NULL && obj->orbit != NULL)
	{
		vector_delete(obj->orbit, string_dealloc);
		free(obj->orbit);
	}
}

set parse_input(const input* input)
{
	set set = set_create(sizeof(object), input->lines.size, 2.0f, object_hash, object_equals);
	vector_iter it = vector_iterator(&input->lines);
	string* line;
	while((line = vector_iter_next(&it)) != NULL)
	{
		string_pair pair = string_split_char(*line, ')');
		object key = (object) { .id = pair.a, .orbit = NULL };
		object* obj = set_at(&set, &key);
		if(obj != NULL)
		{
			vector_push(obj->orbit, &pair.b);
			string_delete(&pair.a);
			continue;
		}
		key.orbit = vector_new(sizeof(string));
		vector_push(key.orbit, &pair.b);
		set_insert(&set, &key);
	}
	return set;
}
