#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef size_t (*set_hash)(void* element);
typedef size_t (*set_equality)(void* a, void* b);

struct slot;

//Hashset which can be used as hashmap by using a struct with key/value pair and custom hash/equality functions
//Stores copied elements
typedef struct
{
	struct slot** buckets;
	size_t element_size;
	size_t size;
	size_t capacity;
	set_hash hash;
	set_equality equality;
} set;

typedef struct
{
	set* set;
	size_t bucket;
	struct slot* slot;
} set_iter;

//New empty hashset with elements of size 'element_size', 'bucket_count' buckets (non resizable)
//'hash' function is optional, default hashes based on bytes
//'equality' is optional, default compares bytes
set set_create(size_t element_size, size_t bucket_count, set_hash hash, set_equality equality);
//Delete all elements in the hashset
void set_delete(set* set);

//Insert 'element' into the set if it's not already in the set. Returns true if it was added
bool set_insert(set* set, void* element);
//Return true if the set contains 'element'
bool set_contains(const set* set, void* element);
//Remove 'element' from the set. Returns true if it was removed
bool set_remove(set* set, void* element);
//Peek at the memory of the full element of a key. This can be used to make the set work as a hashmap. Returns NULL if not found
void* set_at(set* set, void* element);
//Return a copy of the full element of a key. This can be used to make the set work as a hashmap. Returns NULL if not found
void* set_at_cpy(const set* set, void* element);
//Peek at the memory of the element at index. Iterating in a loop from '0' to 'set.size - 1' is safe, but using 'set_iterator' is faster
void* set_at_index(set* set, size_t index);

//Setup an iterator for the hashset. Use 'set_iter_next' to get the next element in a loop
set_iter set_iterator(set* set);
//Move to the next element of the set iterator. Returns NULL if the end is reached
void* set_iter_next(set_iter* iter);
