#include "vector.h"
#include <stdlib.h>
#include <string.h>

static void ensure_capacity(vector* v, size_t min_capacity)
{
	if(v->capacity < min_capacity)
	{
		vector_set_capacity(v, v->capacity * 2);
	}
}

vector vector_create(size_t element_size)
{
	vector v = { .data = NULL, .element_size = element_size, .size = 0, .capacity = 0 };
	vector_set_capacity(&v, 4);
	return v;
}

vector vector_from(size_t element_size, void* elements, size_t elements_len)
{
	vector v = vector_create(element_size);
	for(size_t i = 0; i < elements_len; i++)
	{
		vector_push(&v, (uint8_t*)elements + element_size * i);
	}
	return v;
}

vector* vector_new(size_t element_size)
{
	vector v = vector_create(element_size);
	vector* v_ptr = malloc(sizeof(vector));
	memcpy(v_ptr, &v, sizeof(vector));
	return v_ptr;
}

void vector_delete(vector* v, vector_element_dealloc dealloc)
{
	if(dealloc != NULL)
	{
		for(size_t i = 0; i < v->size; i++)
		{
			dealloc(vector_at(v, i));
		}
	}
	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
}

vector vector_clone(const vector* v, vector_element_copy copy)
{
	vector new_vec = vector_create(v->element_size);
	if(v->capacity > 4)
	{
		vector_set_capacity(&new_vec, v->capacity);
	}
	if(copy != NULL)
	{
		for(size_t i = 0; i < v->size; i++)
		{
			void* cpy = copy(vector_at(v, i));
			vector_push(&new_vec, cpy);
			free(cpy);
		}
	}
	else
	{
		memcpy(new_vec.data, v->data, v->element_size * v->size);
		new_vec.size = v->size;
	}
	return new_vec;
}

void vector_set_capacity(vector* v, size_t capacity)
{
	if(capacity < v->size)
	{
		return;
	}
	void* old = v->data;
	v->data = malloc(v->element_size * capacity);
	v->capacity = capacity;
	if(old != NULL)
	{
		memcpy(v->data, old, v->element_size * v->size);
	}
}

void vector_clear(vector* v, vector_element_dealloc dealloc)
{
	for(int64_t i = v->size - 1; i >= 0; i--)
	{
		vector_remove_at(v, i, dealloc);
	}
	v->size = 0;
}

void vector_push(vector* v, void* element)
{
	ensure_capacity(v, v->size + 1);
	memcpy(v->data + v->size * v->element_size, element, v->element_size);
	v->size++;
}

void vector_set(vector* v, size_t index, void* element, vector_element_dealloc dealloc)
{
	void* prev = vector_at(v, index);
	if(dealloc != NULL && prev != NULL)
	{
		dealloc(prev);
	}
	memcpy(prev, element, v->element_size);
}

void* vector_at(vector* v, size_t index)
{
	return v->data + v->element_size * index;
}

void* vector_at_cpy(const vector* v, size_t index)
{
	void* data = malloc(v->element_size);
	memcpy(v->data + v->element_size * index, data, v->element_size);
	return data;
}

void* vector_first(vector* v)
{
	return vector_at(v, 0);
}

void* vector_last(vector* v)
{
	return vector_at(v, v->size - 1);
}

void vector_insert(vector* v, size_t index, void* element)
{
	if(index == v->size)
	{
		vector_push(v, element);
		return;
	}

	ensure_capacity(v, v->size + 1);
	memcpy(v->data + (index + 1) * v->element_size, v->data + index * v->element_size, (v->size - index) * v->element_size);
	memcpy(v->data + index * v->element_size, element, v->element_size);
	v->size++;
}

void vector_remove_at(vector* v, size_t index, vector_element_dealloc dealloc)
{
	if(dealloc != NULL)
	{
		dealloc(vector_at(v, index));
	}
	memcpy(v->data + index * v->element_size, v->data + (index + 1) * v->element_size, (v->size - index - 1) * v->element_size);
	memset(v->data + (v->size - 1) * v->element_size, 0, v->element_size);
	v->size--;
}

size_t vector_index_of(const vector* v, void* element)
{
	for(size_t i = 0; i < v->size; i++)
	{
		if(memcmp(v->data + i * v->element_size, element, v->element_size) == 0)
		{
			return i;
		}
	}
	return -1;
}

size_t vector_index_of_pred(const vector* v, vector_pred predicate)
{
	for(size_t i = 0; i < v->size; i++)
	{
		if(predicate(i, v->data + i * v->element_size))
		{
			return i;
		}
	}
	return -1;
}

bool vector_all(const vector* v, vector_pred predicate)
{
	for(size_t i = 0; i < v->size; i++)
	{
		if(!predicate(i, v->data + i * v->element_size))
		{
			return false;
		}
	}
	return true;
}

bool vector_any(const vector* v, vector_pred predicate)
{
	for(size_t i = 0; i < v->size; i++)
	{
		if(predicate(i, v->data + i * v->element_size))
		{
			return true;
		}
	}
	return false;
}

bool vector_count(const vector* v, vector_pred predicate)
{
	size_t c = 0;
	for(size_t i = 0; i < v->size; i++)
	{
		if(predicate(i, v->data + i * v->element_size))
		{
			c++;
		}
	}
	return c;
}

vector_iter vector_iterator(vector* v)
{
	return (vector_iter) { .vector = v, .index = -1 };
}

void* vector_iter_next(vector_iter* iter)
{
	iter->index++;
	if(iter->index < iter->vector->size)
	{
		return vector_at(iter->vector, iter->index);
	}
	return NULL;
}

void vector_dealloc(void* v)
{
	vector_delete(v, NULL);
}
