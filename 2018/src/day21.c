#include "solutions.h"
#include "common.h"
#include "chronal_device.h"

result day21_part1(const input* input)
{
	int32_t ip_reg = 0;
	vector instructions = chronal_parse_input(input, &ip_reg);

	int32_t res = 0;
	int32_t regs[6] = { 0 };
	while(regs[ip_reg] >= 0 && regs[ip_reg] < instructions.size)
	{
		const instr* instruction = vector_at_c(&instructions, regs[ip_reg]);

		if(memcmp(instruction->op, "eqrr", 4) == 0 && (instruction->a == 0 || instruction->b == 0))
		{
			res = instruction->a != 0 ? regs[instruction->a] : regs[instruction->b];
			break;
		}

		chronal_execute_instr(instruction, ip_reg, regs);
		regs[ip_reg]++;
	}

	vector_delete(&instructions, NULL);
	return result_int(res);
}

result day21_part2(const input* input)
{
	int32_t ip_reg = 0;
	vector instructions = chronal_parse_input(input, &ip_reg);

	int32_t last_unique = 0;
	bool* seen = calloc(1 << 24, sizeof(bool));

	int32_t res = 0;
	int32_t regs[6] = { 0 };
	while(regs[ip_reg] >= 0 && regs[ip_reg] < instructions.size)
	{
		const instr* instruction = vector_at_c(&instructions, regs[ip_reg]);

		if(memcmp(instruction->op, "eqrr", 4) == 0 && (instruction->a == 0 || instruction->b == 0))
		{
			int32_t cmp = instruction->a != 0 ? regs[instruction->a] : regs[instruction->b];
			if(seen[cmp])
			{
				res = last_unique;
				break;
			}

			seen[cmp] = true;
			last_unique = cmp;
		}

		chronal_execute_instr(instruction, ip_reg, regs);
		regs[ip_reg]++;
	}

	free(seen);
	vector_delete(&instructions, NULL);
	return result_int(res);
}
