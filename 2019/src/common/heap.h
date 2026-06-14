#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef int (*heap_compare)(const void* a, const void* b);
typedef void (*heap_dealloc)(void* element);

//Heap which can be used as priority queue by using a struct with key/value pair and custom compare/dealloc functions
//Stores copied elements
typedef struct
{
	uint8_t* data;
	size_t element_size;
	size_t size;
	size_t capacity;
	heap_compare compare;
	heap_dealloc dealloc;
	uint8_t* temp_element;
} heap;

//New empty heap with elements of size 'element_size'
//'compare' function is mandatory
//'dealloc' function is optional, default does nothing
heap heap_create(size_t element_size, heap_compare compare, heap_dealloc dealloc);
//Delete all elements in the heap
void heap_delete(heap* heap);
//Remove all elements in the heap
void heap_clear(heap* heap);

//Increase the capacity of the heap to at least 'capacity', returns new capacity, does nothing if capacity is already sufficient
size_t heap_reserve(heap* heap, size_t capacity);
//Add a copy of 'element' to the heap
bool heap_push(heap* heap, const void* element);
//Remove the top element from the heap
bool heap_pop(heap* heap);
//Peek at the top element of the heap, returns pointer to internal copy, do not modify or free this pointer
const void* heap_peek(const heap* heap);
//Return pointer to element at 'index', do not modify or free this pointer
const void* heap_at(const heap* heap, size_t index);

#endif
