#include "solutions.h"
#include "common.h"

static const size_t GRID_SIZE = 300;

static int64_t power_level(int64_t serial_number, size_t x, size_t y);

result day11_part1(const input* input)
{
	const size_t square_size = 3;
	int64_t serial_number = strtoll(input->lines_c[0].data, NULL, 10);

	int64_t max_power = INT64_MIN;
	size_t max_power_x = 0, max_power_y = 0;
	for(size_t x = 1; x <= GRID_SIZE - square_size + 1; x++)
	{
		for(size_t y = 1; y <= GRID_SIZE - square_size + 1; y++)
		{
			int64_t total_power = 0;
			for(size_t i = 0; i < square_size; i++)
			{
				for(size_t k = 0; k < square_size; k++)
				{
					total_power += power_level(serial_number, x + i, y + k);
				}
			}
			if(total_power > max_power)
			{
				max_power = total_power;
				max_power_x = x;
				max_power_y = y;
			}
		}
	}

	return result_fmt("%zu,%zu", max_power_x, max_power_y);
}

result day11_part2(const input* input)
{
	int64_t serial_number = strtoll(input->lines_c[0].data, NULL, 10);

	int64_t* cache = malloc(GRID_SIZE * GRID_SIZE * sizeof(int64_t));
	for(size_t y = 0; y < GRID_SIZE; y++)
	{
		for(size_t x = 0; x < GRID_SIZE; x++)
		{
			cache[y * GRID_SIZE + x] = power_level(serial_number, x + 1, y + 1);
		}
	}

	int64_t max_power = INT64_MIN;
	size_t max_power_x = 0, max_power_y = 0;
	size_t max_power_size = 0;
	for(size_t x = 1; x <= GRID_SIZE; x++)
	{
		for(size_t y = 1; y <= GRID_SIZE; y++)
		{
			size_t remaining_x = GRID_SIZE - x + 1;
			size_t remaining_y = GRID_SIZE - y + 1;
			size_t max_square_size = remaining_x < remaining_y ? remaining_x : remaining_y;
			int64_t total_power = 0;
			for(size_t i = 0; i < max_square_size; i++)
			{
				for(size_t y_off = 0; y_off <= i; y_off++)
				{
					total_power += cache[(y + y_off - 1) * GRID_SIZE + (x + i - 1)];
				}
				for(size_t x_off = 0; x_off < i; x_off++)
				{
					total_power += cache[(y + i - 1) * GRID_SIZE + (x + x_off - 1)];
				}
				if(total_power > max_power)
				{
					max_power = total_power;
					max_power_x = x;
					max_power_y = y;
					max_power_size = i + 1;
				}
			}
		}
	}

	free(cache);
	return result_fmt("%zu,%zu,%zu", max_power_x, max_power_y, max_power_size);
}

int64_t power_level(int64_t serial_number, size_t x, size_t y)
{
	int64_t rack_id = x + 10;
	int64_t power = rack_id * y;
	power += serial_number;
	power *= rack_id;
	power = (power / 100) % 10;
	power -= 5;
	return power;
}
