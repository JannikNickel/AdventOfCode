#include "solutions.h"
#include "common.h"
#include <inttypes.h>

typedef struct marble
{
	uint64_t value;
	struct marble* next;
	struct marble* prev;
} marble;

static size_t solve(const input* input, uint64_t multiplier);
static marble* marble_append(marble* curr, marble* new_marble, uint64_t value);
static marble* marble_remove(marble* curr);

result day09_part1(const input* input)
{
	return result_int(solve(input, 1));
}

result day09_part2(const input* input)
{
	return result_int(solve(input, 100));
}

size_t solve(const input* input, uint64_t multiplier)
{
	size_t players = 0;
	uint64_t last_marble = 0;
	sscanf(input->raw.data, "%zu players; last marble is worth " SFMT_UINT64_T " points", &players, &last_marble);
	last_marble *= multiplier;

	marble* marble_pool = malloc(sizeof(marble) * (last_marble + 1));
	marble* curr_marble = &marble_pool[0];
	curr_marble->value = 0;
	curr_marble->prev = curr_marble;
	curr_marble->next = curr_marble;

	size_t curr_player = 0;
	size_t max_score = 0;
	size_t* scores = calloc(players, sizeof(size_t));
	for(uint64_t next_marble = 1; next_marble <= last_marble; next_marble++)
	{
		if(next_marble % 23ULL == 0)
		{
			marble* to_remove = curr_marble;
			for(size_t i = 0; i < 7; i++)
			{
				to_remove = to_remove->prev;
			}
			scores[curr_player] += next_marble + to_remove->value;
			max_score = scores[curr_player] > max_score ? scores[curr_player] : max_score;
			curr_marble = marble_remove(to_remove);
		}
		else
		{
			curr_marble = marble_append(curr_marble->next, &marble_pool[next_marble], next_marble);
		}
		curr_player = (curr_player + 1) % players;
	}

	free(marble_pool);
	free(scores);
	return max_score;
}

marble* marble_append(marble* curr, marble* new_marble, uint64_t value)
{
	new_marble->value = value;
	new_marble->prev = curr;
	new_marble->next = curr->next;
	curr->next->prev = new_marble;
	curr->next = new_marble;
	return new_marble;
}

marble* marble_remove(marble* curr)
{
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	return curr->next;
}
