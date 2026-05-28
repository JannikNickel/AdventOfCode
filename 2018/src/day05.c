#include "solutions.h"
#include "common.h"

typedef struct polymer
{
	char unit;
	struct polymer* prev;
	struct polymer* next;
} polymer;

static polymer* build_polymer(const string* input);
static polymer* clone_polymer(polymer* poly);
static void react_polymer(polymer** poly, char wildcard);
static void append_unit(polymer** head, polymer** tail, char unit);
static void remove_unit(polymer* unit);
static size_t free_polymer(polymer* poly);

result day05_part1(const input* input)
{
	polymer* poly = build_polymer(&input->raw);
	react_polymer(&poly, 0);
	size_t result = free_polymer(poly);
	return result_int(result);
}

result day05_part2(const input* input)
{
	polymer* original_poly = build_polymer(&input->raw);
	react_polymer(&original_poly, 0); // React once for performance, since the order of reactions doesn't matter for part 2

	size_t shortest = SIZE_MAX;
	for(char unit = 'a'; unit <= 'z'; unit++)
	{
		polymer* poly = clone_polymer(original_poly);
		react_polymer(&poly, unit);
		size_t size = free_polymer(poly);
		shortest = size < shortest ? size : shortest;
	}

	free_polymer(original_poly);
	return result_int(shortest);
}

polymer* build_polymer(const string* input)
{
	polymer* head = NULL;
	polymer* tail = NULL;
	for(size_t i = 0; i < input->length; i++)
	{
		append_unit(&head, &tail, input->data[i]);
	}
	return head;
}

static polymer* clone_polymer(polymer* poly)
{
	polymer* head = NULL;
	polymer* tail = NULL;
	while(poly != NULL)
	{
		append_unit(&head, &tail, poly->unit);
		poly = poly->next;
	}
	return head;
}

void react_polymer(polymer** poly, char wildcard)
{
	polymer* curr = *poly;
	while(curr != NULL)
	{
		char curr_u = curr->unit;
		char next_u = curr->next != NULL ? curr->next->unit : 0;
		bool react = curr->next != NULL && ((curr_u >= 'a' && curr_u <= 'z' && next_u == curr_u - 32) || (curr_u >= 'A' && curr_u <= 'Z' && next_u == curr_u + 32));
		bool wildcard_react = wildcard != 0 && (curr_u == wildcard || curr_u == wildcard - 32);
		if(react || wildcard_react)
		{
			polymer* prev_poly = curr->prev;
			polymer* next_poly = !wildcard_react ? curr->next->next : curr->next;
			if(!wildcard_react)
			{
				remove_unit(curr->next);
			}
			remove_unit(curr);
			curr = prev_poly != NULL ? prev_poly : next_poly;
			if(prev_poly == NULL)
			{
				*poly = next_poly;
			}
			continue;
		}
		curr = curr->next;
	}
}

void append_unit(polymer** head, polymer** tail, char unit)
{
	polymer* new_unit = malloc(sizeof(polymer));
	new_unit->unit = unit;
	new_unit->prev = *tail;
	new_unit->next = NULL;
	if(*tail != NULL)
	{
		(*tail)->next = new_unit;
	}
	else
	{
		*head = new_unit;
	}
	*tail = new_unit;
}

void remove_unit(polymer* unit)
{
	if(unit->prev != NULL)
	{
		unit->prev->next = unit->next;
	}
	if(unit->next != NULL)
	{
		unit->next->prev = unit->prev;
	}
	free(unit);
}

size_t free_polymer(polymer* poly)
{
	size_t size = 0;
	while(poly != NULL)
	{
		polymer* next = poly->next;
		free(poly);
		poly = next;
		size++;
	}
	return size;
}
