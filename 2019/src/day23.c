#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

#define COMPUTERS 50

typedef struct
{
	size_t inst_ptr;
	size_t rel_base;
	vector* code;
	vector* input;
	vector* output;
	vector* queue;
} computer;

static int64_t run_network(const string code_str, bool nat);

result day23_part1(const input* input)
{
	int64_t res = run_network(input->raw, false);
	return result_int(res);
}

result day23_part2(const input* input)
{
	int64_t res = run_network(input->raw, true);
	return result_int(res);
}

int64_t run_network(const string code_str, bool nat)
{
	vector code = intcode_parse(code_str);
	int64_t nat_data[2];
	int64_t last_nat_y = INT64_MIN;

	computer pcs[COMPUTERS];
	for(int64_t i = 0; i < COMPUTERS; i++)
	{
		vector* clone = vector_new(sizeof(int64_t));
		for(size_t i = 0; i < code.size; i++)
		{
			vector_push(clone, vector_at(&code, i));
		}
		pcs[i].code = clone;
		pcs[i].input = vector_new(sizeof(int64_t));
		pcs[i].output = vector_new(sizeof(int64_t));
		pcs[i].queue = vector_new(sizeof(int64_t));
		vector_push(pcs[i].input, &i);

		intcode_run_result res = intcode_continue(pcs[i].code, pcs[i].input, pcs[i].output, 0, 0, false, false);
		pcs[i].inst_ptr = res.inst_ptr;
		pcs[i].rel_base = res.rel_base;
	}

	int64_t result = 0;
	while(result == 0)
	{
		bool idle = true;
		for(int i = 0; i < COMPUTERS; i++)
		{
			computer* pc = &pcs[i];
			if(pc->queue->size > 0)
			{
				idle = false;
				for(size_t i = 0; i < pc->queue->size; i++)
				{
					vector_push(pc->input, vector_at(pc->queue, i));
				}
				vector_clear(pc->queue, NULL);
			}
			else
			{
				vector_push(pc->input, &(int64_t) { -1 });
			}
			intcode_run_result res = intcode_continue(pc->code, pc->input, pc->output, pc->inst_ptr, pc->rel_base, false, false);
			pc->inst_ptr = res.inst_ptr;
			pc->rel_base = res.rel_base;

			for(size_t k = 0; k < pc->output->size; k += 3)
			{
				int64_t addr = *(int64_t*)vector_at(pc->output, k + 0);
				int64_t x = *(int64_t*)vector_at(pc->output, k + 1);
				int64_t y = *(int64_t*)vector_at(pc->output, k + 2);

				if(addr == 255)
				{
					if(nat)
					{
						nat_data[0] = x;
						nat_data[1] = y;
						continue;
					}
					else
					{
						result = y;
						break;
					}
				}

				computer* other = &pcs[addr];
				vector_push(other->queue, &x);
				vector_push(other->queue, &y);
			}
			vector_clear(pc->output, NULL);
		}

		if(idle)
		{
			computer* target = &pcs[0];
			vector_push(target->queue, &nat_data[0]);
			vector_push(target->queue, &nat_data[1]);
			if(last_nat_y == nat_data[1])
			{
				result = last_nat_y;
				break;
			}
			last_nat_y = nat_data[1];
		}
	}

	for(size_t i = 0; i < COMPUTERS; i++)
	{
		computer* pc = &pcs[i];
		vector_delete(pc->code, NULL);
		vector_delete(pc->input, NULL);
		vector_delete(pc->output, NULL);
		vector_delete(pc->queue, NULL);
		free(pc->code);
		free(pc->input);
		free(pc->output);
		free(pc->queue);
	}
	vector_delete(&code, NULL);
	return result;
}
