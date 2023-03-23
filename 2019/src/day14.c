#include "solutions.h"
#include "common.h"

static const uint64_t ORE_CAPACITY = 1000000000000;

typedef struct
{
	string name;
	uint64_t amount;
} ingredient;

typedef struct
{
	ingredient result;
	vector* ingredients;
} recipe;

typedef ingredient item;

static bool use(const set* recipes, set* inventory, item itm);
static bool refine(const set* recipes, set* inventory, item itm);
static size_t str_key_hash(const string* element);
static bool str_key_equals(const string* a, const string* b);
static void recipe_dealloc(recipe* recipe);
static void ingredient_dealloc(ingredient* ingredient);
static set parse_input(const input* input);
static ingredient parse_ingredient(const char* str);

result day14_part1(const input* input)
{
	set recipes = parse_input(input);
	set inventory = set_create(sizeof(item), recipes.size * 2, 0.75f, str_key_hash, str_key_equals);

	string ore = string_from("ORE");
	string fuel = string_from("FUEL");
	set_insert(&inventory, &(item) { .name = ore, .amount = ORE_CAPACITY });
	refine(&recipes, &inventory, (item) { .name = fuel, .amount = 1 });
	uint64_t res = ORE_CAPACITY - ((item*)set_at(&inventory, &(item) {.name = ore }))->amount;

	string_delete(&ore);
	string_delete(&fuel);
	set_delete(&recipes, recipe_dealloc);
	set_delete(&inventory, NULL);
	return result_uint(res);
}

result day14_part2(const input* input)
{
	set recipes = parse_input(input);
	set inventory = set_create(sizeof(item), recipes.size * 2, 0.75f, str_key_hash, str_key_equals);

	string ore = string_from("ORE");
	string fuel = string_from("FUEL");
	set_insert(&inventory, &(item) {.name = ore, .amount = ORE_CAPACITY });

	refine(&recipes, &inventory, (item) { .name = fuel, .amount = 1 });
	item* ore_inv = set_at(&inventory, &(item) {.name = ore });
	uint64_t one_fuel_req = ORE_CAPACITY - ore_inv->amount;
	while(refine(&recipes, &inventory, (item) { .name = fuel, .amount = max(1, ore_inv->amount / one_fuel_req) })) { }
	uint64_t res = ((item*)set_at(&inventory, &(item) {.name = fuel }))->amount;

	string_delete(&ore);
	string_delete(&fuel);
	set_delete(&recipes, recipe_dealloc);
	set_delete(&inventory, NULL);
	return result_uint(res);
}

bool use(const set* recipes, set* inventory, item itm)
{
	set_insert(inventory, &(item) { .name = itm.name, .amount = 0 });
	item* invItem = set_at(inventory, &itm);
	if(invItem->amount >= itm.amount || refine(recipes, inventory, (item) { .name = itm.name, .amount = itm.amount - invItem->amount }))
	{
		invItem->amount -= itm.amount;
		return true;
	}
	return false;
}

bool refine(const set* recipes, set* inventory, item itm)
{
	if(strcmp(itm.name.data, "ORE") == 0)
	{
		return false;
	}

	recipe* r = set_at(recipes, &(recipe) { .result = (ingredient) { .name = itm.name } });
	uint64_t use_amount = (itm.amount + r->result.amount - 1) / r->result.amount;
	for(size_t i = 0; i < r->ingredients->size; i++)
	{
		ingredient* ingredient = vector_at(r->ingredients, i);
		if(!use(recipes, inventory, (item) { .name = ingredient->name, .amount = use_amount * ingredient->amount }))
		{
			return false;
		}
	}

	set_insert(inventory, &(item) { .name = itm.name, .amount = 0 });
	((item*)set_at(inventory, &itm))->amount += use_amount * r->result.amount;
	return true;
}

size_t str_key_hash(const string* element)
{
	return string_hash(*element);
}

bool str_key_equals(const string* a, const string* b)
{
	return string_equals(*a, *b);
}

void recipe_dealloc(recipe* recipe)
{
	ingredient_dealloc(&recipe->result);
	vector_delete(recipe->ingredients, ingredient_dealloc);
	free(recipe->ingredients);
}

void ingredient_dealloc(ingredient* ingredient)
{
	string_delete(&ingredient->name);
}

set parse_input(const input* input)
{
	set recipes = set_create(sizeof(recipe), 128, 0.75f, str_key_hash, str_key_equals);
	for(size_t i = 0; i < input->lines.size; i++)
	{
		string* line = vector_at(&input->lines, i);
		string_pair pair = string_split_cstr(*line, " => ");
		vector parts = string_split_all_cstr(pair.a, ", ");
		vector* ingredients = vector_new(sizeof(ingredient));
		for(size_t i = 0; i < parts.size; i++)
		{
			string* p = (string*)vector_at(&parts, i);
			ingredient ingredient = parse_ingredient(p->data);
			vector_push(ingredients, &ingredient);
		}
		ingredient result = parse_ingredient(pair.b.data);
		recipe r = (recipe) { .result = result, .ingredients = ingredients };
		set_insert(&recipes, &r);

		string_pair_delete(&pair);
		vector_delete(&parts, string_dealloc);
	}
	return recipes;
}

ingredient parse_ingredient(const char* str)
{
	char* c = strchr(str, ' ');
	size_t index = (size_t)(c - str);
	return (ingredient) { .name = string_from(str + index + 1), .amount = strtol(str, NULL, 10) };
}
