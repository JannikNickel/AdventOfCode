#include "solutions.h"
#include "common.h"

static const int WIDTH = 25;
static const int HEIGHT = 6;

typedef struct
{
	char* data;
} layer;

static vector parse_input(const input* input);
static int64_t count_zeros(const layer* l);

result day08_part1(const input* input)
{
	vector layers = parse_input(input);
	layer min = *(layer*)vector_min(&layers, count_zeros);
	uint32_t one = 0, two = 0;
	for(size_t i = 0; i < WIDTH * HEIGHT; i++)
	{
		one += min.data[i] == '1';
		two += min.data[i] == '2';
	}
	vector_delete(&layers, NULL);
	return result_int(one * two);
}

result day08_part2(const input* input)
{
	vector layers = parse_input(input);

	size_t res_size = (WIDTH + 1) * HEIGHT;
	char* res = malloc(sizeof(char) * res_size);
	res[res_size - 1] = '\0';
	size_t res_index = 0;
	for(size_t i = 0; i < WIDTH * HEIGHT; i++)
	{
		for(size_t k = 0; k < layers.size; k++)
		{
			layer* l = vector_at(&layers, k);
			if(l->data[i] != '2')
			{
				res[res_index++] = l->data[i] == '1' ? '@' : ' ';
				if((i + 1) % WIDTH == 0 && (i + 1) != WIDTH * HEIGHT)
				{
					res[res_index++] = '\n';
				}
				break;
			}
		}
	}
	vector_delete(&layers, NULL);
	return result_string(res);
}

vector parse_input(const input* input)
{
	vector v = vector_create(sizeof(layer));
	int layers = input->raw.length / (WIDTH * HEIGHT);
	for(size_t i = 0; i < layers; i++)
	{
		vector_push(&v, &(layer) { .data = input->raw.data + i * (WIDTH * HEIGHT) });
	}
	return v;
}

int64_t count_zeros(const layer* l)
{
	int64_t c = 0;
	for(size_t i = 0; i < WIDTH * HEIGHT; i++)
	{
		c += l->data[i] == '0';
	}
	return c;
}
