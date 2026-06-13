#include "solutions.h"
#include "common.h"
#include "chronal_device.h"

result day19_part1(const input* input)
{
	int32_t ip_reg = 0;
	vector instructions = chronal_parse_input(input, &ip_reg);
	
	int32_t regs[6] = { 0 };
	while(regs[ip_reg] >= 0 && regs[ip_reg] < instructions.size)
	{
		const instr* instruction = vector_at_c(&instructions, regs[ip_reg]);
		chronal_execute_instr(instruction, ip_reg, regs);
		regs[ip_reg]++;
	}
	regs[ip_reg]--;

	return result_int(regs[0]);
}

result day19_part2(const input* input)
{
	// Input instructions do the following
	// 1) Random noise (first instructions contain the actual logic, setup happens later)
	// 2) Initialize one register to high target value
	// 3) Calculate sum of all divisors of that value and store in r0 (with two nested loops)

	int32_t ip_reg = 0;
	vector instructions = chronal_parse_input(input, &ip_reg);

	int32_t regs[6] = { 1 };
	for(size_t i = 0; i < instructions.size; i++)
	{
		if(regs[ip_reg] >= 0 && regs[ip_reg] < instructions.size)
		{
			const instr* instruction = vector_at_c(&instructions, regs[ip_reg]);
			chronal_execute_instr(instruction, ip_reg, regs);
			regs[ip_reg]++;
		}
	}

	int32_t large_reg_val = 0;
	for(size_t i = 0; i < 6; i++)
	{
		large_reg_val = max(large_reg_val, regs[i]);
	}

	int32_t res = 0;
	for(int32_t i = 1; i <= large_reg_val; i++)
	{
		if(large_reg_val % i == 0)
		{
			res += i;
		}
	}
	return result_int(res);
}

void chronal_execute_instr(const instr* instruction, int32_t ip_reg, int32_t regs[6])
{
	int32_t inp_a = chronal_is_immediate_a(instruction->op) ? instruction->a : regs[instruction->a];
	int32_t inp_b = chronal_is_immediate_b(instruction->op) ? instruction->b : regs[instruction->b];
	int32_t out_c = instruction->c;

	if(memcmp(instruction->op, "addr", 4) == 0 || memcmp(instruction->op, "addi", 4) == 0)
	{
		regs[out_c] = inp_a + inp_b;
	}
	else if(memcmp(instruction->op, "mulr", 4) == 0 || memcmp(instruction->op, "muli", 4) == 0)
	{
		regs[out_c] = inp_a * inp_b;
	}
	else if(memcmp(instruction->op, "banr", 4) == 0 || memcmp(instruction->op, "bani", 4) == 0)
	{
		regs[out_c] = inp_a & inp_b;
	}
	else if(memcmp(instruction->op, "borr", 4) == 0 || memcmp(instruction->op, "bori", 4) == 0)
	{
		regs[out_c] = inp_a | inp_b;
	}
	else if(memcmp(instruction->op, "setr", 4) == 0 || memcmp(instruction->op, "seti", 4) == 0)
	{
		regs[out_c] = inp_a;
	}
	else if(memcmp(instruction->op, "gtir", 4) == 0 || memcmp(instruction->op, "gtri", 4) == 0 || memcmp(instruction->op, "gtrr", 4) == 0)
	{
		regs[out_c] = inp_a > inp_b ? 1 : 0;
	}
	else if(memcmp(instruction->op, "eqir", 4) == 0 || memcmp(instruction->op, "eqri", 4) == 0 || memcmp(instruction->op, "eqrr", 4) == 0)
	{
		regs[out_c] = inp_a == inp_b ? 1 : 0;
	}
}

bool chronal_is_immediate_a(const char op[4])
{
	return memcmp(op, "gtir", 4) == 0 || memcmp(op, "eqir", 4) == 0 || memcmp(op, "seti", 4) == 0;
}

bool chronal_is_immediate_b(const char op[4])
{
	return memcmp(op, "addi", 4) == 0 || memcmp(op, "muli", 4) == 0 || memcmp(op, "bani", 4) == 0 || memcmp(op, "bori", 4) == 0 || memcmp(op, "gtri", 4) == 0 || memcmp(op, "eqri", 4) == 0;
}

vector chronal_parse_input(const input* input, int32_t* ip)
{
	vector res = vector_create(sizeof(instr));
	for(size_t i = 0; i < input->line_count; i++)
	{
		const string* line = &input->lines_c[i];
		if(line->data[0] == '#')
		{
			*ip = atoi(&line->data[4]);
		}
		else
		{
			instr instruction = { 0 };
			sscanf(line->data, "%4c " SFMT_INT32_T " " SFMT_INT32_T " " SFMT_INT32_T, instruction.op, &instruction.a, &instruction.b, &instruction.c);
			vector_push(&res, &instruction);
		}
	}
	return res;
}
