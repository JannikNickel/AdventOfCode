#include "solutions.h"
#include "common.h"

static bool contains_letter_count(string str, uint32_t target_count);
static const string* find_almost_matching(string str, const string* options, size_t option_count);

result day02_part1(const input* input)
{
	int32_t two_count = 0;
	int32_t three_count = 0;
	for(size_t i = 0; i < input->line_count; i++)
	{
		two_count += contains_letter_count(input->lines_c[i], 2);
		three_count += contains_letter_count(input->lines_c[i], 3);
	}
	return result_int(two_count * three_count);
}

result day02_part2(const input* input)
{
	for(size_t i = 0; i < input->line_count; i++)
	{
		string line = input->lines_c[i];
		const string* matching = find_almost_matching(line, input->lines_c + i + 1, input->line_count - i - 1);
		if(matching != NULL)
		{
			size_t res_idx = 0;
			char* result = malloc(line.length + 1);
			for(size_t k = 0; k < line.length; k++)
			{
				if(string_at(line, k) == string_at(*matching, k))
				{
					result[res_idx++] = string_at(line, k);
				}
			}
			result[res_idx] = '\0';
			return result_heap_string(result);
		}
	}
	return result_none();
}

bool contains_letter_count(string str, uint32_t target_count)
{
	uint32_t counts[26] = { 0 };
	for(size_t i = 0; i < str.length; i++)
	{
		unsigned char c = (unsigned char)string_at(str, i);
		if(c >= 'a' && c <= 'z')
		{
			counts[c - 'a']++;
		}
	}
	for(size_t i = 0; i < 26; i++)
	{
		if(counts[i] == target_count)
		{
			return true;
		}
	}
	return false;
}

const string* find_almost_matching(string str, const string* options, size_t option_count)
{
	for(size_t i = 0; i < option_count; i++)
	{
		const string* option = &options[i];
		size_t diffs = 0;
		for(size_t k = 0; k < str.length; k++)
		{
			diffs += string_at(str, k) != string_at(*option, k);
		}
		if(diffs == 1)
		{
			return option;
		}
	}
	return NULL;
}
