#include "set.h"
#include <stdlib.h>
#include <string.h>

struct slot
{
	size_t element_hash;
	void* element;
	struct slot* next;
};

size_t default_hash(void* element, size_t element_size)
{
	size_t h = 15127993;
	for(size_t i = 0; i < element_size; i++)
	{
		uint8_t c = *((uint8_t*)element + i);
		h = h * 5737 + c;
	}
	return h;
}

size_t calc_hash(set* set, void* element)
{
	return set->hash != NULL ? set->hash(element) : default_hash(element, set->element_size);
}

size_t default_equality(void* a, void* b, size_t element_size)
{
	return memcmp(a, b, element_size) == 0;
}

size_t test_equality(set* set, void* a, void* b)
{
	return set->equality != NULL ? set->equality(a, b) : default_equality(a, b, set->element_size);
}

struct slot** get_bucket(set* set, size_t hash)
{
	return &set->buckets[(hash & (set->capacity - 1))];
}

struct slot* find_slot(set* set, void* element)
{
	size_t h = calc_hash(set, element);
	struct slot* slot = *get_bucket(set, h);
	while(slot != NULL)
	{
		if(slot->element != NULL)
		{
			if(slot->element_hash == h && test_equality(set, slot->element, element))
			{
				return slot;
			}
		}
		slot = slot->next;
	}
	return NULL;
}

void insert_back_of_slot(struct slot* slot, void* element, size_t hash, size_t element_size)
{
	while(slot->next != NULL)
	{
		slot = slot->next;
	}
	struct slot* s = malloc(sizeof(struct slot));
	s->element = malloc(element_size);
	memcpy(s->element, element, element_size);
	s->element_hash = hash;
	s->next = NULL;
	slot->next = s;
}

set set_create(size_t element_size, size_t bucket_count, set_hash hash, set_equality equality)
{
	set s;
	s.buckets = (struct slot**)calloc(bucket_count, sizeof(struct slot*));
	s.size = 0;
	s.element_size = element_size;
	s.capacity = bucket_count;
	s.hash = hash;
	s.equality = equality;
	return s;
}

void set_delete(set* set)
{
	for(size_t i = 0; i < set->size; i++)
	{
		set_remove(set, set_at_index(set, i));
	}
	free(set->buckets);
	set->buckets = NULL;
	set->capacity = 0;
	set->size = 0;
}

bool set_insert(set* set, void* element)
{
	if(set_contains(set, element))
	{
		return false;
	}

	size_t h = calc_hash(set, element);
	struct slot** slot_ptr = get_bucket(set, h);
	if(*slot_ptr == NULL)
	{
		struct slot* s = malloc(sizeof(struct slot));
		s->element = malloc(set->element_size);
		memcpy(s->element, element, set->element_size);
		s->element_hash = h;
		s->next = NULL;
		*slot_ptr = s;
	}
	else
	{
		insert_back_of_slot(*slot_ptr, element, h, set->element_size);
	}
	set->size++;
	return true;
}

bool set_contains(const set* set, void* element)
{
	return find_slot(set, element) != NULL;
}

bool set_remove(set* set, void* element)
{
	size_t h = calc_hash(set, element);
	struct slot** slot_ptr = get_bucket(set, h);

	struct slot* current = *slot_ptr;
	if(current == NULL)
	{
		return false;
	}

	while(current != NULL)
	{
		if(current->element_hash == h && test_equality(set, current->element, element))
		{
			*slot_ptr = current->next;
			free(current->element);
			free(current);
			set->size--;
			return true;
		}
		slot_ptr = &current->next;
		current = current->next;
	}
	return false;
}

void* set_at(set* set, void* element)
{
	struct slot* s = find_slot(set, element);
	if(s != NULL)
	{
		return s->element;
	}
	return NULL;
}

void* set_at_cpy(const set* set, void* element)
{
	struct slot* s = find_slot(set, element);
	if(s != NULL)
	{
		void* data = malloc(set->element_size);
		memcpy(data, s->element, set->element_size);
		return data;
	}
	return NULL;
}

void* set_at_index(set* set, size_t index)
{
	size_t c = 0;
	for(size_t i = 0; i < set->capacity; i++)
	{
		struct slot* s = set->buckets[i];
		while(s != NULL)
		{
			if(c == index)
			{
				return s->element;
			}
			s = s->next;
			c++;
		}
	}
	return NULL;
}

set_iter set_iterator(set* set)
{
	set_iter iter;
	iter.set = set;
	iter.bucket = 0;
	iter.slot = NULL;
	return iter;
}

void* set_iter_next(set_iter* iter)
{
	if(iter->slot == NULL)
	{
		iter->slot = iter->set->buckets[0];
	}
	else
	{
		while(iter->bucket < iter->set->capacity)
		{
			if(iter->slot != NULL && iter->slot->next != NULL)
			{
				iter->slot = iter->slot->next;
				break;
			}
			iter->bucket++;
			iter->slot = iter->bucket < iter->set->capacity ? iter->set->buckets[iter->bucket] : NULL;
			if(iter->slot != NULL)
			{
				break;
			}
		}
	}
	return iter->slot != NULL ? iter->slot->element : NULL;
}
