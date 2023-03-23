#include "set.h"
#include <stdlib.h>
#include <string.h>

struct set_slot
{
	size_t element_hash;
	void* element;
	struct set_slot* next;
};

static size_t default_hash(void* element, size_t element_size)
{
	size_t h = 15127993;
	for(size_t i = 0; i < element_size; i++)
	{
		uint8_t c = *((uint8_t*)element + i);
		h = h * 5737 + c;
	}
	return h;
}

static size_t calc_hash(set* set, void* element)
{
	return set->hash != NULL ? set->hash(element) : default_hash(element, set->element_size);
}

static bool default_equality(void* a, void* b, size_t element_size)
{
	return memcmp(a, b, element_size) == 0;
}

static size_t test_equality(set* set, void* a, void* b)
{
	return set->equality != NULL ? set->equality(a, b) : default_equality(a, b, set->element_size);
}

static struct set_slot** get_bucket(set* set, size_t hash)
{
	return &set->buckets[(hash & (set->capacity - 1))];
}

static struct set_slot* find_slot(set* set, void* element)
{
	size_t h = calc_hash(set, element);
	struct set_slot* slot = *get_bucket(set, h);
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

static void insert_back_of_slot(struct set_slot* slot, void* element, size_t hash, size_t element_size)
{
	while(slot->next != NULL)
	{
		slot = slot->next;
	}
	struct set_slot* s = malloc(sizeof(struct set_slot));
	s->element = malloc(element_size);
	memcpy(s->element, element, element_size);
	s->element_hash = hash;
	s->next = NULL;
	slot->next = s;
}

static void insert_internal(set* set, void* element)
{
	size_t h = calc_hash(set, element);
	struct set_slot** slot_ptr = get_bucket(set, h);
	if(*slot_ptr == NULL)
	{
		struct set_slot* s = malloc(sizeof(struct set_slot));
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
}

static void set_resize(set* set)
{
	size_t old_capacity = set->capacity;
	size_t new_capacity = old_capacity * 2;
	struct set_slot** old = set->buckets;

	set->buckets = (struct set_slot**)calloc(new_capacity, sizeof(struct set_slot*));
	set->capacity = new_capacity;
	set->size = 0;

	for(size_t i = 0; i < old_capacity; i++)
	{
		struct set_slot* slot = old[i];
		while(slot != NULL)
		{
			if(slot->element != NULL)
			{
				//Insert and remove old
				insert_internal(set, slot->element);
				free(slot->element);
			}
			struct set_slot* old = slot;
			slot = slot->next;
			free(old);
		}
	}
}

set set_create(size_t element_size, size_t bucket_count, float resize_factor, set_hash hash, set_equality equality)
{
	if(bucket_count < 4)
	{
		bucket_count = 32;
	}
	set s;
	s.buckets = (struct set_slot**)calloc(bucket_count, sizeof(struct set_slot*));
	s.size = 0;
	s.element_size = element_size;
	s.capacity = bucket_count;
	s.resize_factor = resize_factor;
	s.hash = hash;
	s.equality = equality;
	return s;
}

void set_delete(set* set, set_element_dealloc dealloc)
{
	for(size_t i = 0; i < set->size; i++)
	{
		set_remove(set, set_at_index(set, 0), dealloc);
	}
	free(set->buckets);
	set->buckets = NULL;
	set->capacity = 0;
	set->size = 0;
}

void set_clear(set* set, set_element_dealloc dealloc)
{
	for(size_t i = 0; i < set->size; i++)
	{
		set_remove(set, set_at_index(set, 0), dealloc);
	}
}

bool set_insert(set* set, void* element)
{
	if(set_contains(set, element))
	{
		return false;
	}

	float load_factor = set->size / (double)set->capacity;
	if(load_factor > set->resize_factor)
	{
		set_resize(set);
	}

	insert_internal(set, element);
	return true;
}

bool set_contains(const set* set, void* element)
{
	return find_slot(set, element) != NULL;
}

bool set_remove(set* set, void* element, set_element_dealloc dealloc)
{
	size_t h = calc_hash(set, element);
	struct set_slot** slot_ptr = get_bucket(set, h);

	struct set_slot* current = *slot_ptr;
	if(current == NULL)
	{
		return false;
	}

	while(current != NULL)
	{
		if(current->element_hash == h && test_equality(set, current->element, element))
		{
			*slot_ptr = current->next;

			if(dealloc != NULL)
			{
				dealloc(current->element);
			}
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
	struct set_slot* s = find_slot(set, element);
	if(s != NULL)
	{
		return s->element;
	}
	return NULL;
}

void* set_at_cpy(const set* set, void* element)
{
	struct set_slot* s = find_slot(set, element);
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
		struct set_slot* s = set->buckets[i];
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
	iter.bucket = -1;
	iter.slot = NULL;
	return iter;
}

void* set_iter_next(set_iter* iter)
{
	if(iter->bucket == -1)
	{
		iter->bucket = 0;
		iter->slot = iter->set->buckets[iter->bucket];
		if(iter->slot != NULL)
		{
			return iter->slot->element;
		}
	}
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
	return iter->slot != NULL ? iter->slot->element : NULL;
}
