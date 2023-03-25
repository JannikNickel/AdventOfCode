#include "solutions.h"
#include "common.h"

typedef enum
{
	STACK,
	CUT,
	INCREMENT,
} mode;

typedef struct
{
	mode mode;
	int32_t amount;
} order;

static const uint64_t CARDS = 10007;
static const uint64_t TARGET = 2019;

static void stack(uint64_t* cards);
static void cut(uint64_t* cards, int32_t amount);
static void increment(uint64_t* cards, int32_t step);
static vector parse_input(const input* input);

result day22_part1(const input* input)
{
	vector orders = parse_input(input);
	uint64_t* cards = malloc(sizeof(uint64_t) * CARDS);
	if(cards == NULL)
	{
		return;
	}
	for(uint64_t i = 0; i < CARDS; i++)
	{
		cards[i] = i;
	}

	for(size_t i = 0; i < orders.size; i++)
	{
		order* order = vector_at(&orders, i);
		switch(order->mode)
		{
			case STACK:		stack(cards); break;
			case CUT:		cut(cards, order->amount); break;
			case INCREMENT: increment(cards, order->amount); break;
			default: break;
		}
	}

	uint64_t res = 0;
	for(uint64_t i = 0; i < CARDS; i++)
	{
		if(cards[i] == TARGET)
		{
			res = i;
			break;
		}
	}
	vector_delete(&orders, NULL);
	return result_uint(res);
}

result day22_part2(const input* input)
{
	return result_none();
}

void stack(uint64_t* cards)
{
	for(size_t i = 0; i < CARDS / 2; i++)
	{
		int64_t temp = cards[i];
		cards[i] = cards[CARDS - i - 1];
		cards[CARDS - i - 1] = temp;
	}
}

void cut(uint64_t* cards, int32_t amount)
{
	size_t start = 0;
	if(amount < 0)
	{
		amount = abs(amount);
		start = CARDS - amount;
	}
	uint64_t* cut = malloc(sizeof(uint64_t) * amount);
	size_t ci = start;
	for(size_t i = 0; i < amount; i++)
	{
		cut[i] = cards[ci++];
	}
	if(start == 0)
	{
		//shift left
		for(size_t i = amount; i < CARDS; i++)
		{
			cards[i - amount] = cards[i];
		}
	}
	else
	{
		//shift right
		for(size_t i = CARDS - amount - 1; i >= 0 && i != (size_t)(-1); i--)
		{
			cards[i + amount] = cards[i];
		}
	}
	start = start == 0 ? CARDS - amount : 0;
	for(size_t i = 0; i < amount; i++)
	{
		cards[start++] = cut[i];
	}
	free(cut);
}

void increment(uint64_t* cards, int32_t step)
{
	uint64_t* order = malloc(sizeof(uint64_t) * CARDS);
	for(size_t i = 0; i < CARDS; i++)
	{
		size_t index = i * step % CARDS;
		order[index] = cards[i];
	}
	memcpy(cards, order, sizeof(uint64_t) * CARDS);
	free(order);
}

vector parse_input(const input* input)
{
	const char* keywords[] = { "stack", "cut", "increment" };
	vector v = vector_create(sizeof(order));
	for(size_t i = 0; i < input->lines.size; i++)
	{
		string* line = vector_at(&input->lines, i);
		mode m;
		size_t num_index = 0;
		for(int i = 0; i < 3; i++)
		{
			if((num_index = string_find_cstr(*line, keywords[i], 0)) != -1)
			{
				num_index += strlen(keywords[i]);
				m = (mode)i;
				break;
			}
		}
		vector_push(&v, &(order) {.mode = m, .amount = strtol(line->data + num_index, NULL, 10) });
	}
	return v;
}
