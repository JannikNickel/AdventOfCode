#include "solutions.h"
#include "common.h"

static void next_recipes(vector* recipes, size_t* elf1, size_t* elf2);

result day14_part1(const input* input)
{
	size_t scores = 0;
	sscanf(input->raw.data, "%zu", &scores);

	size_t elf1 = 0, elf2 = 1;
	vector recipes = vector_create(sizeof(char));
	vector_push(&recipes, &(char){ '3' });
	vector_push(&recipes, &(char){ '7' });

	while(recipes.size < scores + 10)
	{
		next_recipes(&recipes, &elf1, &elf2);
	}

	char result[11] = { 0 };
	memcpy(result, &recipes.data[scores], 10);
	vector_delete(&recipes, NULL);
	return result_string(result);
}

result day14_part2(const input* input)
{
	size_t cmp_len = strlen(input->raw.data);

	size_t elf1 = 0, elf2 = 1;
	vector recipes = vector_create(sizeof(char));
	vector_push(&recipes, &(char){ '3' });
	vector_push(&recipes, &(char){ '7' });

	size_t result = SIZE_MAX;
	while(result == SIZE_MAX)
	{
		next_recipes(&recipes, &elf1, &elf2);
		if(recipes.size >= cmp_len && memcmp(&recipes.data[recipes.size - cmp_len], input->raw.data, cmp_len) == 0)
		{
			result = recipes.size - cmp_len;
		}
		else if(recipes.size > cmp_len && memcmp(&recipes.data[recipes.size - cmp_len - 1], input->raw.data, cmp_len) == 0)
		{
			result = recipes.size - cmp_len - 1;
		}
	}

	vector_delete(&recipes, NULL);
	return result_int(result);
}

void next_recipes(vector* recipes, size_t* elf1, size_t* elf2)
{
	int r0 = *(char*)vector_at(recipes, *elf1) - '0';
	int r1 = *(char*)vector_at(recipes, *elf2) - '0';
	int sum = r0 + r1;
	if(sum >= 10)
	{
		vector_push(recipes, &(char){ (sum / 10) + '0' });
	}
	vector_push(recipes, &(char){ (sum % 10) + '0' });
	*elf1 = (*elf1 + 1 + r0) % recipes->size;
	*elf2 = (*elf2 + 1 + r1) % recipes->size;
}
