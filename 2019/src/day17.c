#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

static const int OFF[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

static vector read_map(string code);
static char get_cell(const vector* map, int x, int y, int width, int height);

result day17_part1(const input* input)
{
	vector lines = read_map(input->raw);
	size_t height = lines.size;
	size_t width = ((vector*)vector_at(&lines, 0))->size;

	int sum = 0;
	for(size_t i = 1; i < height - 1; i++)
	{
		for(size_t k = 1; k < width - 1; k++)
		{
			if(*(char*)vector_at(vector_at(&lines, i), k) != '#')
			{
				continue;
			}
			int neighbours = 0;
			for(size_t l = 0; l < 4; l++)
			{
				vec2 p = vec2_new(k + OFF[l][0], i + OFF[l][1]);
				if(*(char*)vector_at(vector_at(&lines, p.y), p.x) == '#')
				{
					neighbours++;
				}
			}
			if(neighbours >= 4)
			{
				sum += i * k;
			}
		}
	}

	vector_delete(&lines, vector_dealloc);
	return result_int(sum);
}

result day17_part2(const input* input)
{
	vector lines = read_map(input->raw);
	size_t height = lines.size;
	size_t width = ((vector*)vector_first(&lines))->size;

	vec2 origin;
	for(size_t i = 0; i < height; i++)
	{
		for(size_t k = 0; k < width; k++)
		{
			char c = get_cell(&lines, k, i, width, height);
			if(c != '.' && c != '#')
			{
				origin = vec2_new(k, i);
				break;
			}
		}
	}

	vector path = vector_create(sizeof(int));
	vec2 p = origin;
	char c = get_cell(&lines, origin.x, origin.y, width, height);
	int dir = c == '<' ? 0 : (c == '^' ? 1 : (c == '>' ? 2 : 3));
	int targetDir = -1;
	for(int i = 0; i < 4; i++)
	{
		if(get_cell(&lines, origin.x + OFF[i][0], origin.y + OFF[i][1], width, height) == '#')
		{
			targetDir = i;
			break;
		}
	}
	vector_push(&path, &(int){ -(int)'R' });
	dir = targetDir;
	while(true)
	{
		int forward = 0;
		while(get_cell(&lines, p.x + OFF[dir][0], p.y + OFF[dir][1], width, height) == '#')
		{
			p = vec2_add(p, vec2_new(OFF[dir][0], OFF[dir][1]));
			forward++;
		}
		vector_push(&path, &forward);

		//Rotate
		int r_dir = (dir + 4 - 1) % 4;
		int l_dir = (dir + 1) % 4;
		if(get_cell(&lines, p.x + OFF[r_dir][0], p.y + OFF[r_dir][1], width, height) == '#')
		{
			vector_push(&path, &(int){ -(int)'R' });
			dir = r_dir;
		}
		else if(get_cell(&lines, p.x + OFF[l_dir][0], p.y + OFF[l_dir][1], width, height) == '#')
		{
			vector_push(&path, &(int){ -(int)'L' });
			dir = l_dir;
		}
		else
		{
			break;
		}
	}

	char* p_str = calloc(path.size * 3, sizeof(char));
	int p_str_index = 0;
	for(int i = 0; i < path.size; i++)
	{
		int t = *(int*)vector_at(&path, i);
		if(t < 0)
		{
			sprintf(p_str + p_str_index++, "%c", (char)(-t));
		}
		else
		{
			sprintf(p_str + p_str_index, "%d", t);
			p_str_index = strlen(p_str);
		}
		if(i < path.size - 1)
		{
			sprintf(p_str + p_str_index++, ",");
		}
	}

	//Easy to solve by hand in a text editor
	//printf("%s\n", p_str);

	char P[] = "A,B,A,C,B,C,A,B,A,C\n";
	char A[] = "R,6,L,10,R,8,R,8\n";
	char B[] = "R,12,L,8,L,10\n";
	char C[] = "R,12,L,10,R,6,L,10\n";
	char* IN[] = { P, A, B, C };
	string code_str = string_copy(input->raw);
	*string_at_ref(code_str, 0) = '2';
	vector in = vector_create(sizeof(int64_t));
	for(size_t i = 0; i < 4; i++)
	{
		char* arr = IN[i];
		for(size_t i = 0; i < strlen(arr); i++)
		{
			vector_push(&in, &(int64_t) { arr[i] });
		}
	}
	vector_push(&in, &(int64_t) { 'n' });
	vector_push(&in, &(int64_t) { '\n' });

	vector output = intcode_run_simple(code_str, (int64_t*)in.data, in.size);
	int64_t res = *(int64_t*)vector_at(&output, output.size - 1);

	vector_delete(&path, NULL);
	string_delete(&code_str, NULL);
	vector_delete(&lines, vector_dealloc);
	vector_delete(&in, NULL);
	vector_delete(&output, NULL);
	return result_int(res);
}

vector read_map(string code)
{
	vector output = intcode_run_simple(code, NULL, 0);
	vector lines = vector_create(sizeof(vector));

	vector line = vector_create(sizeof(char));
	for(size_t i = 0; i < output.size; i++)
	{
		char c = *(int64_t*)vector_at(&output, i);
		if(c == '\n')
		{
			if(line.size > 0)
			{
				vector_push(&lines, &line);
				line = vector_create(sizeof(char));
			}
			continue;
		}
		vector_push(&line, &c);
	}

	vector_delete(&output, NULL);
	return lines;
}

char get_cell(const vector* map, int x, int y, int width, int height)
{
	if(x < 0 || y < 0 || x >= width || y >= height)
	{
		return '.';
	}
	vector* str = ((vector*)vector_at(map, y));
	return *(char*)vector_at(str, x);
}
