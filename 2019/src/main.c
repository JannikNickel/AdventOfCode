#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "common/str.h"
#include "common/vector.h"
#include "common/set.h"

size_t hash(void* element)
{
	int v = *(int*)element;
	return v;
}

size_t equals(void* a, void* b)
{
	int va = *(int*)a;
	int vb = *(int*)b;
	return va == vb;
}

typedef struct
{
	size_t key;
	char str[64];
} pair;

size_t pair_hash(void* element)
{
	return ((pair*)element)->key;
}

size_t pair_equality(void* a, void* b)
{
	return ((pair*)a)->key == ((pair*)b)->key;
}

pair pair_create(size_t key, char str[64])
{
	pair p;
	p.key = key;
	strcpy_s(p.str, 64, str);
	return p;
}

pair pair_key(size_t key)
{
	pair p;
	p.key = key;
	return p;
}

int main(int argc, char* argv[])
{
	/*string s = string_from("Hello World!");
	size_t i = string_find_char(s, '_', 0);
	bool x = i == -1;

	string_pair pair = string_split_char(s, 'H');

	string_append_cstr(&s, " :)");
	printf("%s\n", s.data);
	printf("%s\n", string_sub(s, 0, 5).data);
	string_delete(&s);*/

	/*vector v = vector_create(sizeof(int));
	for(size_t i = 0; i < 10; i++)
	{
		vector_push(&v, &i);
	}

	int x = 777;
	vector_insert(&v, 2, &x);
	vector_remove_at(&v, 3);

	for(size_t i = 0; i < v.size; i++)
	{
		printf("%d\n", *((int*)vector_at(&v, i)));
	}

	printf("index = %d\n", vector_index_of(&v, &x));
	printf("index pred = %d\n", vector_index_of_pred(&v, is_777));*/

	/*set s = set_create(sizeof(int), 4, NULL, NULL);
	for(int i = 0; i < 8; i++)
	{
		set_insert(&s, &i);
	}

	int x = 4;
	set_remove(&s, &x);
	x = 0;
	set_remove(&s, &x);

	for(int i = 0; i < 10; i++)
	{
		printf("contains %d? %d\n", i, set_contains(&s, &i));
	}

	set_iter it = set_iterator(&s);
	void* element;
	while((element = set_iter_next(&it)) != NULL)
	{
		printf("%d, ", *(size_t*)element);
	}

	printf("\n%d\n", s.size);*/

	/*set s = set_create(sizeof(pair), 4, pair_hash, pair_equality);
	pair p0 = pair_create(0, "First");
	pair p1 = pair_create(1, "Second");
	pair p2 = pair_create(2, "Third");
	set_insert(&s, &p0);
	set_insert(&s, &p1);
	set_insert(&s, &p2);

	pair k0 = pair_key(1);
	printf("%d\n", set_contains(&s, &k0));
	printf("%s\n", ((pair*)set_at(&s, &k0))->str);
	set_delete(&s);*/

	/*string s = string_from("Hello_World!");
	s = string_replace(s, "!", "LL");
	printf("%s|\n", s.data);*/
}
