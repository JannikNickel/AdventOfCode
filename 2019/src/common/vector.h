#pragma once
#include <stdbool.h>
#include <stdint.h>

//Stores copied elements in contiguous memory in the heap
//'size' always contains the current amount of elements
typedef struct
{
	uint8_t* data;
	size_t element_size;
	size_t size;
	size_t capacity;
} vector;

typedef bool (*vector_pred)(size_t index, void* element);

//New empty vector with elements of size 'element_size'
vector vector_create(size_t element_size);
//New empty vector with elements of size 'element_size'
vector* vector_new(size_t element_size);
//Delete the content of the vector
void vector_delete(vector* v);

//Set the capacity of the vector. Only works if 'capacity' >= 'v->size'
void vector_set_capacity(vector* v, size_t capacity);
//Set the size of the vector to 0
void vector_clear(vector* v);

//Add an element to the end of this vector. The element is copied and should be of 'element_size'
void vector_push(vector* v, void* element);
//Peek at the memory of the element at 'index'
void* vector_at(vector* v, size_t index);
//Return a copy of the element at 'index'
void* vector_at_cpy(const vector* v, size_t index);
//Insert an element at 'index'. The element is copied and should be of 'element_size'
void vector_insert(vector* v, size_t index, void* element);
//Remove the element at 'index'
void vector_remove_at(vector* v, size_t index);
//Find the index of an element. Returns -1 (size_t max) if not found
size_t vector_index_of(const vector* v, void* element);
//Find the index of an element by predicate. Returns -1 (size_t max) if not found
size_t vector_index_of_pred(const vector* v, vector_pred predicate);
//Test if all elements in the vector fulfill a condition
bool vector_all(const vector* v, vector_pred predicate);
//Test if all elements in the vector fulfill a condition
bool vector_any(const vector* v, vector_pred predicate);
//Count the amount of elements that fulfill a condition
bool vector_count(const vector* v, vector_pred predicate);
