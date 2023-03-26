#include "solutions.h"
#include "common.h"
#include "bigint/bigint.h"

typedef enum
{
	STACK, CUT, INCREMENT,
} mode;

typedef struct
{
	mode mode;
	int32_t amount;
} order;

static void stack(int32_t* cards, int32_t n_cards);
static void cut(int32_t* cards, int32_t amount, int32_t n_cards);
static void increment(int32_t* cards, int32_t step, int32_t n_cards);
static bigint bigint_new();
static int64_t bigint_to_i64(bigint v);
static bigint bigint_from_i64(int64_t v);
static int64_t mod(int64_t a, int64_t b);
static void bmod(bigint* a, bigint* b);
static bigint inv_mod(int64_t n, int64_t m);
static void bigint_free_many(bigint* numbers, int amount);
static vector parse_input(const input* input);

result day22_part1(const input* input)
{
	vector orders = parse_input(input);
	const int32_t CARDS = 10007;
	const int32_t TARGET = 2019;

	int32_t* cards = malloc(sizeof(int32_t) * CARDS);
	if(cards == NULL)
	{
		return;
	}
	for(int32_t i = 0; i < CARDS; i++)
	{
		cards[i] = i;
	}

	for(size_t i = 0; i < orders.size; i++)
	{
		order* order = vector_at(&orders, i);
		switch(order->mode)
		{
			case STACK:		stack(cards, CARDS); break;
			case CUT:		cut(cards, order->amount, CARDS); break;
			case INCREMENT: increment(cards, order->amount, CARDS); break;
			default: break;
		}
	}

	int32_t res = 0;
	while(res < CARDS && cards[res] != TARGET) { res++; }
	free(cards);
	vector_delete(&orders, NULL);
	return result_int(res);
}

result day22_part2(const input* input)
{
	vector orders = parse_input(input);
	const int64_t CARDS = 119315717514047;
	const int64_t REPEATS = 101741582076661;
	const int64_t TARGET_CARD = 2020;

	int64_t inc = 1;
	int64_t off = 0;
	for(size_t i = 0; i < orders.size; i++)
	{
		order* order = vector_at(&orders, i);
		switch(order->mode)
		{
			case STACK:
				inc = mod(inc * -1, CARDS);
				off = mod(off + inc, CARDS);
				break;
			case CUT:
				bigint c_off = bigint_from_i64(off);
				bigint c_inc = bigint_from_i64(inc);
				bigint c_amount = bigint_from_i64(order->amount);
				bigint c_cards = bigint_from_i64(CARDS);
				bigint_mul(&c_amount, &c_amount, &c_inc);
				bigint_add(&c_off, &c_off, &c_amount);
				bmod(&c_off, &c_cards);
				off = bigint_to_i64(c_off);
				bigint_free_many(&(bigint[]) { c_off, c_inc, c_amount, c_cards }, 3);
				break;
			case INCREMENT:
				bigint i_inv = inv_mod(order->amount, CARDS);
				bigint i_inc = bigint_from_i64(inc);
				bigint i_cards = bigint_from_i64(CARDS);
				bigint_mul(&i_inc, &i_inc, &i_inv);
				bmod(&i_inc, &i_cards);
				inc = bigint_to_i64(i_inc);
				bigint_free_many(&(bigint[]) { i_inv, i_inc, i_cards }, 3);
				break;
			default:
				break;
		}
	}

	bigint increment = bigint_new();
	bigint b_inc = bigint_from_i64(inc);
	bigint b_rep = bigint_from_i64(REPEATS);
	bigint b_cards = bigint_from_i64(CARDS);
	bigint_pow_mod(&increment, &b_inc, &b_rep, &b_cards);
	int64_t increment64 = bigint_to_i64(increment);

	bigint offset = bigint_new();
	bigint b_off = bigint_from_i64(off);
	bigint b_inv = inv_mod(mod((1 - inc), CARDS), CARDS);
	bigint b_one_min_inc = bigint_from_i64(1 - increment64);
	bigint_mul(&offset, &b_off, &b_one_min_inc);
	bigint_mul(&offset, &offset, &b_inv);
	bmod(&offset, &b_cards);

	bigint target = bigint_from_i64(TARGET_CARD);
	bigint_mul(&target, &target, &increment);
	bigint_add(&target, &target, &offset);
	bmod(&target, &b_cards);

	int64_t res = bigint_to_i64(target);
	bigint_free_many(&(bigint[]) { increment, b_inc, b_rep, b_cards, offset, b_off, b_inv, b_one_min_inc, target }, 9);
	vector_delete(&orders, NULL);
	return result_int(res);
}

void stack(int32_t* cards, int32_t n_cards)
{
	for(size_t i = 0; i < n_cards / 2; i++)
	{
		int64_t temp = cards[i];
		cards[i] = cards[n_cards - i - 1];
		cards[n_cards - i - 1] = temp;
	}
}

void cut(int32_t* cards, int32_t amount, int32_t n_cards)
{
	size_t start = 0;
	if(amount < 0)
	{
		amount = abs(amount);
		start = n_cards - amount;
	}
	int32_t* cut = malloc(sizeof(int32_t) * amount);
	size_t ci = start;
	for(size_t i = 0; i < amount; i++)
	{
		cut[i] = cards[ci++];
	}
	if(start == 0)
	{
		for(size_t i = amount; i < n_cards; i++)
		{
			cards[i - amount] = cards[i];
		}
	}
	else
	{
		for(size_t i = n_cards - amount - 1; i >= 0 && i != (size_t)(-1); i--)
		{
			cards[i + amount] = cards[i];
		}
	}
	start = start == 0 ? n_cards - amount : 0;
	for(size_t i = 0; i < amount; i++)
	{
		cards[start++] = cut[i];
	}
	free(cut);
}

void increment(int32_t* cards, int32_t step, int32_t n_cards)
{
	int32_t* order = malloc(sizeof(int32_t) * n_cards);
	for(size_t i = 0; i < n_cards; i++)
	{
		size_t index = i * step % n_cards;
		order[index] = cards[i];
	}
	memcpy(cards, order, sizeof(int32_t) * n_cards);
	free(order);
}

bigint bigint_new()
{
	bigint v;
	bigint_init(&v);
	bigint_from_int(&v, 0);
	return v;
}

int64_t bigint_to_i64(bigint v)
{
	char str[40];
	bigint_write(str, 40, &v);
	return strtoll(str, NULL, 10);
}

bigint bigint_from_i64(int64_t v)
{
	char str[20];
	sprintf(str, "%lld", v);
	bigint res = bigint_new();
	bigint_from_str(&res, str);
	return res;
}

int64_t mod(int64_t a, int64_t b)
{
	return (a % b + b) % b;
}

void bmod(bigint* a, bigint* b)
{
	bigint_mod(a, a, b);
	bigint_add(a, a, b);
	bigint_mod(a, a, b);
}

bigint inv_mod(int64_t n, int64_t m)
{
	bigint res = bigint_new();
	bigint b_n = bigint_from_i64(n);
	bigint b_m_min_2 = bigint_from_i64(m - 2);
	bigint b_m = bigint_from_i64(m);
	bigint_pow_mod(&res, &b_n, &b_m_min_2, &b_m);
	bigint_free(&b_n);
	bigint_free(&b_m_min_2);
	bigint_free(&b_m);
	return res;
}

void bigint_free_many(bigint* numbers, int amount)
{
	for(int i = 0; i < amount; i++)
	{
		bigint_free(&numbers[i]);
	}
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
