#include "solutions.h"
#include "common.h"

typedef struct
{
	vec2 pos;
	vec2 vel;
} light;

static int64_t solve(const input* input, string* message);
static void simulate_step(light* lights, size_t light_count, int32_t mult);
static int64_t determine_entropy(const light* lights, size_t light_count);
static void calc_bounds(const light* lights, size_t light_count, int32_t* min_x, int32_t* max_x, int32_t* min_y, int32_t* max_y);
static string draw_lights(const light* lights, size_t light_count);
static light* parse_input(const input* input, size_t* light_count);

result day10_part1(const input* input)
{
	string res = { NULL };
	solve(input, &res);
	return result_heap_string(res.data);
}

result day10_part2(const input* input)
{
	return result_int(solve(input, NULL));
}

int64_t solve(const input* input, string* message)
{
	size_t light_count;
	light* lights = parse_input(input, &light_count);

	int64_t seconds = 0;
	int64_t last_entropy = INT64_MAX;
	while(true)
	{
		simulate_step(lights, light_count, 1);
		int64_t entropy = determine_entropy(lights, light_count);
		if(last_entropy < entropy)
		{
			if(message)
			{
				simulate_step(lights, light_count, -1);
				*message = draw_lights(lights, light_count);
			}
			break;
		}
		seconds++;
		last_entropy = entropy;
	}

	free(lights);
	return seconds;
}

void simulate_step(light* lights, size_t light_count, int32_t mult)
{
	for(size_t i = 0; i < light_count; i++)
	{
		lights[i].pos.x += lights[i].vel.x * mult;
		lights[i].pos.y += lights[i].vel.y * mult;
	}
}

int64_t determine_entropy(const light* lights, size_t light_count)
{
	int32_t min_x, max_x, min_y, max_y;
	calc_bounds(lights, light_count, &min_x, &max_x, &min_y, &max_y);
	return ((int64_t)max_x - min_x) * ((int64_t)max_y - min_y);
}

void calc_bounds(const light* lights, size_t light_count, int32_t* min_x, int32_t* max_x, int32_t* min_y, int32_t* max_y)
{
	*min_x = INT32_MAX;
	*max_x = INT32_MIN;
	*min_y = INT32_MAX;
	*max_y = INT32_MIN;
	for(size_t i = 0; i < light_count; i++)
	{
		*min_x = min(*min_x, lights[i].pos.x);
		*max_x = max(*max_x, lights[i].pos.x);
		*min_y = min(*min_y, lights[i].pos.y);
		*max_y = max(*max_y, lights[i].pos.y);
	}
}

string draw_lights(const light* lights, size_t light_count)
{
	int32_t min_x, max_x, min_y, max_y;
	calc_bounds(lights, light_count, &min_x, &max_x, &min_y, &max_y);
	size_t width = max_x - min_x + 1;
	size_t height = max_y - min_y + 1;

	size_t str_size = (width + 1) * height + 1;
	char* buffer = malloc(str_size);
	memset(buffer, '.', str_size - 1);
	buffer[str_size - 1] = '\0';

	for(size_t i = 0; i < height; i++)
	{
		buffer[(i + 1) * (width + 1) - 1] = '\n';
	}

	for(size_t i = 0; i < light_count; i++)
	{
		int32_t x = lights[i].pos.x - min_x;
		int32_t y = lights[i].pos.y - min_y;
		if(x >= 0 && x < width && y >= 0 && y < height)
		{
			buffer[y * (width + 1) + x] = '#';
		}
	}
	return string_wrap(buffer);
}

light* parse_input(const input* input, size_t* light_count)
{
	*light_count = input->line_count;
	light* lights = malloc(sizeof(light) * input->line_count);
	for(size_t i = 0; i < input->line_count; i++)
	{
		const string line = input->lines_c[i];
		int32_t x, y, vx, vy;
		sscanf(line.data, "position=<" SFMT_INT32_T ", " SFMT_INT32_T "> velocity=<" SFMT_INT32_T ", " SFMT_INT32_T ">", &x, &y, &vx, &vy);
		lights[i] = (light) { .pos = { x, y }, .vel = { vx, vy } };
	}
	return lights;
}
