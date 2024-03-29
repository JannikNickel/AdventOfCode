#ifndef STR_H
#define STR_H

#include <stdlib.h>
#include "vector.h"

//Dynamic string with data on the heap
typedef struct
{
	char* data;
	size_t length;
} string;

typedef struct
{
	string a;
	string b;
} string_pair;

//New empty string (data on heap)
string string_empty();
//New whitespace string
string string_whitespace(size_t len);
//New string (data on heap) from c-string
string string_from(const char* c_str);
//New string (data on heap) from a section of a c-string
string string_from_sub(const char* c_str, size_t start, size_t length);
//Wraps the c string as data into a new string instance
string string_wrap(const char* c_str);
//New heap allocated string (data on heap) from c-string
string* string_new(const char* c_str);
//New string with copied data from another string
string string_copy(string s);
//Delete the content of a string
void string_delete(string* s);
//Delete both strings of a pair
void string_pair_delete(string_pair* pair);

//Append the rhs string to the lhs string ptr (modifies lhs)
void string_append(string* lhs, const string rhs);
//Append the rhs c-string to the lhs string ptr (modifies lhs)
void string_append_cstr(string* lhs, const char* rhs);
//Append the char to the lhs string ptr (modifies lhs)
void string_append_char(string* lhs, char c);

//Get the char at 'index'
char string_at(string s, size_t index);
//Get the char ptr at 'index' to modify it
char* string_at_ref(string s, size_t index);
//Find the index of a char in a string beginning at index 'start'. Returns -1 (size_t max) if not found
size_t string_find_char(string s, char c, size_t start);
//Find the start index of a c-substring in a string beginning at index 'start'. Returns -1 (size_t max) if not found
size_t string_find_cstr(string s, const char* search, size_t start);

//New substring from 's' containing 'length' chars beginning at 'start'
string string_sub(string s, size_t start, size_t length);
//Replace all occurrences of a c-string with another c-string
string string_replace(string s, const char* search, const char* replacement);
//Trim all whitespaces from the beginning and end of the string and return a new string. Use 'whitespace_func' to determine which characters should be trimmed, otherwise NULL to trim spaces
string string_trim(string s, bool (*whitespace_func)(char));

//Split the string at the first occurrence of 'c'. Returns a pair of two empty strings if 'c' is not in 's'
string_pair string_split_char(string s, char c);
//Split the string by the first occurrence of 'search'. Returns a pair of two empty strings if 'search' is not in 's'
string_pair string_split_cstr(string s, const char* search);
//Create a vector of strings containing all splits between the 'search' string
vector string_split_all_cstr(string s, const char* search);

//Generate a hash for a string
size_t string_hash(string s);
//Generate a hash for a string
size_t string_hash_c(const char* s);
//Generate a hash for a string
bool string_equals(string a, string b);

//Free the data of a string as void*. Can be used as dealloc function when deleting a vector of strings
void string_dealloc(void* s);

#endif
