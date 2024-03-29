#ifndef VECTOR_H
#define VECTOR_H

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

typedef struct
{
	vector* vector;
	size_t index;
} vector_iter;

typedef bool (*vector_pred)(size_t index, const void* element);
typedef void (*vector_element_dealloc)(void* element);
typedef void* (*vector_element_copy)(void* element);
typedef int64_t (*vector_element_ord)(const void* element);
typedef int (*vector_element_sort)(const void* a, const void* b);

//New empty vector with elements of size 'element_size'
vector vector_create(size_t element_size);
//New vector with elements of size 'element_size' which copies the 'elements'
vector vector_from(size_t element_size, void* elements, size_t elements_len);
//New empty vector with elements of size 'element_size'
vector* vector_new(size_t element_size);
//Delete the content of the vector. (Optional) pass a function as 'dealloc' to run it for every element before deallocating the memory
void vector_delete(vector* v, vector_element_dealloc dealloc);
//Copy the vector data into a new vector.
//(Optional) pass a function to adjust how elements are copied. This is required for structs that hold additional pointers if you want to create deep copy. The result of the copy is freed after copying it into the vector memory
vector vector_clone(const vector* v, vector_element_copy copy);

//Set the capacity of the vector. Only works if 'capacity' >= 'v->size'
void vector_set_capacity(vector* v, size_t capacity);
//Set the size of the vector to 0. (Optional) pass a function as 'dealloc' to run it for every element before removing it
void vector_clear(vector* v, vector_element_dealloc dealloc);

//Add an element to the end of the vector. The element is copied and should be of 'element_size'
void vector_push(vector* v, void* element);
//Set the 'index'-th element of the vector. The element is copied and should be of 'element_size'. (Optional) pass a function as 'dealloc' to run it before replacing the element
void vector_set(vector* v, size_t index, void* element, vector_element_dealloc dealloc);
//Peek at the memory of the element at 'index'
void* vector_at(vector* v, size_t index);
//Return a copy of the element at 'index'
void* vector_at_cpy(const vector* v, size_t index);
//Peek at the memory of the first element
void* vector_first(vector* v);
//Peek at the memory of the last element
void* vector_last(vector* v);
//Insert an element at 'index'. The element is copied and should be of 'element_size'
void vector_insert(vector* v, size_t index, void* element);
//Remove the element at 'index'. (Optional) pass a function as 'dealloc' to run it for the removed element
void vector_remove_at(vector* v, size_t index, vector_element_dealloc dealloc);
//Remove the first element. (Optional) pass a function as 'dealloc' to run it for the removed element
void vector_remove_first(vector* v, vector_element_dealloc dealloc);
//Remove the last element. (Optional) pass a function as 'dealloc' to run it for the removed element
void vector_remove_last(vector* v, vector_element_dealloc dealloc);
//Find the index of an element. Returns -1 (size_t max) if not found
size_t vector_index_of(const vector* v, void* element);
//Find the index of an element by predicate. Returns -1 (size_t max) if not found
size_t vector_index_of_pred(const vector* v, vector_pred predicate);
//Test if all elements in the vector fulfill a condition
bool vector_all(const vector* v, vector_pred predicate);
//Test if all elements in the vector fulfill a condition
bool vector_any(const vector* v, vector_pred predicate);
//Count the amount of elements that fulfill a condition
size_t vector_count(const vector* v, vector_pred predicate);
//Peek at the memory of the element with the min value by 'ord'
void* vector_min(const vector* v, vector_element_ord ord);
//Peek at the memory of the element with the max value by 'ord'
void* vector_max(const vector* v, vector_element_ord ord);
//Sort the vector in ascending order based on the 'sort' result
void vector_sort(vector* v, vector_element_sort sort);
//Reverse the order of all elements in the vector
void vector_reverse(vector* v);

//Setup an iterator for the hashset. Use 'vec_iter_next' to get the next element in a loop
vector_iter vector_iterator(vector* v);
//Move to the next element of the vec iterator. Returns NULL if the end is reached
void* vector_iter_next(vector_iter* iter);

//Free the data of a vector as void*. Can be used as dealloc function when deleting a vector of vector
void vector_dealloc(void* v);

#endif
