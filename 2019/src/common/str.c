#include "str.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void copy_from_cstr(string* s, const char* c_str, size_t length)
{
	size_t len = length == (size_t)(-1) ? strlen(c_str) : length;
	s->data = malloc(sizeof(char) * (len + 1));
	memcpy(s->data, c_str, len + 1);
	s->data[len] = '\0';
	s->length = len;
}

static void append_from_cstr(string* s, const char* c_str, size_t size)
{
	int len = size == (size_t)(-1) ? strlen(c_str) : size;
	char* old = s->data;
	s->data = malloc(sizeof(char) * (s->length + len + 1));
	memcpy(s->data, old, sizeof(char) * s->length);
	memcpy(s->data + sizeof(char) * s->length, c_str, sizeof(char) * (len + (size == (size_t)(-1) ? 1 : 0)));
	s->length += len;
	free(old);
}

static string_pair split_pair(string s, size_t index, size_t off)
{
	if(index == (size_t)(-1))
	{
		return (string_pair) { .a = string_empty(), .b = string_empty() };
	}
	return (string_pair) { .a = string_sub(s, 0, index), .b = string_sub(s, index + off, s.length - (index + 1)) };
}

string string_empty()
{
	return string_from("");
}

string string_whitespace(size_t len)
{
	string s;
	s.data = malloc(sizeof(char) * (len + 1));
	memset(s.data, ' ', len);
	s.data[len] = '\n';
	s.length = len;
	return s;
}

string string_from(const char* c_str)
{
	string s;
	copy_from_cstr(&s, c_str, -1);
	return s;
}

string string_from_sub(const char* c_str, size_t start, size_t length)
{
	string s;
	copy_from_cstr(&s, c_str + start, length);
}

string string_wrap(const char* c_str)
{
	string s;
	s.data = c_str;
	s.length = strlen(c_str);
	return s;
}

string* string_new(const char* c_str)
{
	string* s = malloc(sizeof(string));
	copy_from_cstr(s, c_str, -1);
	return s;
}

string string_copy(string s)
{
	return string_from(s.data);
}

void string_delete(string* s)
{
	if(s != NULL)
	{
		free(s->data);
		s->data = NULL;
		s->length = 0;
	}
}

void string_pair_delete(string_pair* pair)
{
	if(pair != NULL)
	{
		string_delete(&pair->a);
		string_delete(&pair->b);
	}
}

void string_append(string* lhs, const string rhs)
{
	append_from_cstr(lhs, rhs.data, -1);
}

void string_append_cstr(string* lhs, const char* rhs)
{
	append_from_cstr(lhs, rhs, -1);
}

void string_append_char(string* lhs, char c)
{
	append_from_cstr(lhs, &c, 1);
}

char string_at(string s, size_t index)
{
	return s.data[index];
}

char* string_at_ref(string s, size_t index)
{
	return &s.data[index];
}

size_t string_find_char(string s, char c, size_t start)
{
	char* c_ptr = strchr(s.data + start, c);
	if(c_ptr != NULL)
	{
		return (c_ptr - s.data);
	}
	return -1;
}

size_t string_find_cstr(string s, const char* search, size_t start)
{
	char* s_ptr = strstr(s.data + start, search);
	if(s_ptr != NULL)
	{
		return (s_ptr - s.data);
	}
	return -1;
}

string string_sub(string s, size_t start, size_t length)
{
	if(length == 0)
	{
		return string_empty();
	}
	string s_new;
	s_new.data = malloc(sizeof(char) * (length + 1));
	memcpy(s_new.data, s.data + start, length);
	s_new.data[length] = '\0';
	s_new.length = length;
	return s_new;
}

string string_replace(string s, const char* search, const char* replacement)
{
	string res = string_empty();
	size_t pos = 0;
	size_t prev = 0;
	while((pos = string_find_cstr(s, search, pos)) != (size_t)(-1))
	{
		if(prev < pos)
		{
			append_from_cstr(&res, s.data + prev, pos - prev);
		}
		append_from_cstr(&res, replacement, -1);
		pos += 1;
		prev = pos;
	}
	if(prev < s.length)
	{
		append_from_cstr(&res, s.data + prev, -1);
	}
	if(res.data[res.length] != '\0')
	{
		append_from_cstr(&res, "", -1);
	}
	return res;
}

string string_trim(string s, bool (*whitespace_func)(char))
{
	if(whitespace_func == NULL)
	{
		whitespace_func = isspace;
	}
	size_t start = 0;
	while(whitespace_func(s.data[start]))
	{
		start++;
	}
	size_t end = s.length - 1;
	while(whitespace_func(s.data[end]) && end > start)
	{
		end--;
	}
	return start < end ? string_sub(s, start, end - start) : string_empty();
}

string_pair string_split_char(string s, char c)
{
	return split_pair(s, string_find_char(s, c, 0), 1);
}

string_pair string_split_cstr(string s, const char* search)
{
	return split_pair(s, string_find_cstr(s, search, 0), strlen(search));
}

vector string_split_all_cstr(string s, const char* search)
{
	vector v = vector_create(sizeof(string));
	int len = strlen(search);
	if(len != 0 && s.length != 0)
	{
		size_t start = 0;
		size_t end;
		while((end = string_find_cstr(s, search, start)) != (size_t)(-1))
		{
			string sub = string_sub(s, start, end - start);
			vector_push(&v, &sub);
			start = end + len;
		}
		string sub = string_sub(s, start, s.length - start);
		vector_push(&v, &sub);
	}
	return v;
}

size_t string_hash(string s)
{
	return string_hash_c(s.data);
}

size_t string_hash_c(const char* s)
{
	size_t len = strlen(s);
	size_t h = 15127993;
	for(size_t i = 0; i < len; i++)
	{
		char c = s[i];
		h = h * 5737 + c;
	}
	return h;
}

bool string_equals(string a, string b)
{
	return a.length == b.length && memcmp(a.data, b.data, a.length) == 0;
}

void string_dealloc(void* s)
{
	string_delete(s);
}
