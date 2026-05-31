#include "solutions.h"
#include "common.h"

typedef struct step
{
	char id;
	struct step* dependencies[26];
	size_t dependency_count;
} step;

typedef struct
{
	char task_id;
	size_t remaining_ticks;
} worker;

typedef enum
{
	STATUS_BLOCKED = 0,
	STATUS_IN_PROGRESS = 1,
	STATUS_COMPLETED = 2
} status;

static vector create_dependency_graph(const input* input);
static size_t solve(const vector* dependency_graph, char* result_str, size_t worker_count, size_t fixed_duration);
static void free_dependency_graph(vector* graph);

result day07_part1(const input* input)
{
	vector dependency_graph = create_dependency_graph(input);

	char* result_str = malloc(dependency_graph.size + 1);
	result_str[dependency_graph.size] = '\0';
	solve(&dependency_graph, result_str, 1, 0);

	free_dependency_graph(&dependency_graph);
	return result_heap_string(result_str);
}

result day07_part2(const input* input)
{
	vector dependency_graph = create_dependency_graph(input);
	size_t ticks = solve(&dependency_graph, NULL, 5, 60);
	free_dependency_graph(&dependency_graph);
	return result_int(ticks);
}

vector create_dependency_graph(const input* input)
{
	step* steps[26] = { NULL };
	for(size_t i = 0; i < input->line_count; i++)
	{
		string line = input->lines_c[i];
		char step_id = line.data[36];
		char dependency_id = line.data[5];

		size_t dependency_idx = dependency_id - 'A';
		if(steps[dependency_idx] == NULL)
		{
			steps[dependency_idx] = malloc(sizeof(step));
			(*steps[dependency_idx]) = (step) { .id = dependency_id, .dependencies = { NULL }, .dependency_count = 0 };
		}

		size_t step_idx = step_id - 'A';
		step** s = &steps[step_idx];
		if(*s == NULL)
		{
			*s = malloc(sizeof(step));
			*(*s) = (step) { .id = step_id, .dependencies = { NULL }, .dependency_count = 0 };
		}
		(*s)->dependencies[(*s)->dependency_count++] = steps[dependency_idx];
	}

	vector steps_vec = vector_create(sizeof(step*));
	for(size_t i = 0; i < 26; i++)
	{
		if(steps[i] != NULL)
		{
			vector_push(&steps_vec, &steps[i]);
		}
	}
	return steps_vec;
}

size_t solve(const vector* dependency_graph, char* result_str, size_t worker_count, size_t fixed_duration)
{
	size_t ticks = 0;
	worker* workers = calloc(worker_count, sizeof(worker));

	bool all_idle = true;
	status resolved[26] = { 0 };
	size_t resolved_count = 0;
	while(!all_idle || resolved_count != dependency_graph->size)
	{
		for(size_t i = 0; i < dependency_graph->size; i++)
		{
			const step* s = *(const step**)vector_at_c(dependency_graph, i);
			if(resolved[s->id - 'A'] != STATUS_BLOCKED)
			{
				continue;
			}

			bool res = true;
			for(size_t k = 0; k < s->dependency_count; k++)
			{
				res &= resolved[s->dependencies[k]->id - 'A'] == STATUS_COMPLETED;
			}

			if(res)
			{
				for(size_t k = 0; k < worker_count; k++)
				{
					if(workers[k].task_id == '\0')
					{
						resolved[s->id - 'A'] = STATUS_IN_PROGRESS;
						workers[k].task_id = s->id;
						workers[k].remaining_ticks = fixed_duration + (s->id - 'A' + 1);
						break;
					}
				}
			}
		}

		all_idle = true;
		for(size_t i = 0; i < worker_count; i++)
		{
			if(workers[i].task_id != '\0')
			{
				if(--workers[i].remaining_ticks == 0)
				{
					resolved[workers[i].task_id - 'A'] = STATUS_COMPLETED;
					if(result_str != NULL)
					{
						result_str[resolved_count] = workers[i].task_id;
					}
					resolved_count++;

					workers[i].task_id = '\0';
				}
			}
			all_idle &= workers[i].task_id == '\0';
		}

		ticks++;
	}

	free(workers);
	return ticks;
}

void free_dependency_graph(vector* graph)
{
	for(size_t i = 0; i < graph->size; i++)
	{
		step* s = *(step**)vector_at(graph, i);
		free(s);
	}
	vector_delete(graph, NULL);
}
