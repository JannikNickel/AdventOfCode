#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static uint8_t* heap_at_internal(heap* heap, size_t index)
{
	return heap->data + index * heap->element_size;
}

static bool heap_swap(heap* heap, size_t a, size_t b)
{
	if(a == b)
	{
		return false;
	}

	uint8_t* a_ptr = heap_at_internal(heap, a);
	uint8_t* b_ptr = heap_at_internal(heap, b);
	memcpy(heap->temp_element, a_ptr, heap->element_size);
	memcpy(a_ptr, b_ptr, heap->element_size);
	memcpy(b_ptr, heap->temp_element, heap->element_size);
	return true;
}

static void heap_sift_up(heap* heap, size_t index)
{
	while(index > 0)
	{
		size_t parent = (index - 1) / 2;
		if(heap->compare(heap_at_internal(heap, index), heap_at_internal(heap, parent)) >= 0)
		{
			break;
		}
		heap_swap(heap, index, parent);
		index = parent;
	}
}

static void heap_sift_down(heap* heap, size_t index)
{
	while(true)
	{
		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;
		size_t best = index;

		if(left < heap->size && heap->compare(heap_at_internal(heap, left), heap_at_internal(heap, best)) < 0)
		{
			best = left;
		}
		if(right < heap->size && heap->compare(heap_at_internal(heap, right), heap_at_internal(heap, best)) < 0)
		{
			best = right;
		}

		if(best == index)
		{
			break;
		}
		heap_swap(heap, index, best);
		index = best;
	}
}

heap heap_create(size_t element_size, heap_compare compare, heap_dealloc dealloc)
{
	assert(compare != NULL);
	assert(element_size > 0);

	uint8_t* temp_element = malloc(element_size);
	assert(temp_element != NULL);

	return (heap)
	{
		.data = NULL,
		.element_size = element_size,
		.size = 0,
		.capacity = 0,
		.compare = compare,
		.dealloc = dealloc,
		.temp_element = temp_element
	};
}

void heap_delete(heap* heap)
{
	heap_clear(heap);
	free(heap->data);
	heap->data = NULL;
	heap->capacity = 0;
	heap->size = 0;
	free(heap->temp_element);
	heap->temp_element = NULL;
}

void heap_clear(heap* heap)
{
	if(heap->dealloc != NULL)
	{
		for(size_t i = 0; i < heap->size; i++)
		{
			heap->dealloc(heap->data + i * heap->element_size);
		}
	}
	heap->size = 0;
}

size_t heap_reserve(heap* heap, size_t capacity)
{
	if(capacity <= heap->capacity)
	{
		return heap->capacity;
	}
	if(capacity < 4)
	{
		capacity = 4;
	}
	
	uint8_t* new_data = realloc(heap->data, heap->element_size * capacity);
	if(new_data != NULL)
	{
		heap->data = new_data;
		heap->capacity = capacity;
	}
	return heap->capacity;
}

bool heap_push(heap* heap, const void* element)
{
	if(heap->size == heap->capacity)
	{
		size_t new_capacity = heap->capacity > 0 ? heap->capacity * 2 : 8;
		if(heap_reserve(heap, new_capacity) < new_capacity)
		{
			return false;
		}
	}

	memcpy(heap_at_internal(heap, heap->size), element, heap->element_size);
	heap->size++;
	heap_sift_up(heap, heap->size - 1);
	return true;
}

bool heap_pop(heap* heap)
{
	if(heap->size == 0)
	{
		return false;
	}

	if(heap->dealloc != NULL)
	{
		heap->dealloc(heap_at_internal(heap, 0));
	}

	heap->size--;
	if(heap->size > 0)
	{
		memcpy(heap_at_internal(heap, 0), heap_at_internal(heap, heap->size), heap->element_size);
		heap_sift_down(heap, 0);
	}
	return true;
}

const void* heap_peek(const heap* heap)
{
	return heap_at(heap, 0);
}

const void* heap_at(const heap* heap, size_t index)
{
	if(index >= heap->size)
	{
		return NULL;
	}
	return heap->data + index * heap->element_size;
}
