#include "solutions.h"
#include "common.h"

typedef enum
{
	ADDR, ADDI,
	MULR, MULI,
	BANR, BANI,
	BORR, BORI,
	SETR, SETI,
	GTIR, GTRI, GTRR,
	EQIR, EQRI, EQRR,
	OPCODE_COUNT
} opcode;

typedef struct
{
	int32_t inp_regs[4];
	int32_t instruction[4];
	int32_t out_regs[4];
} instr_test;

typedef struct
{
	int32_t code[4];
} instr;

static void build_instr_map(const vector* tests, opcode map[OPCODE_COUNT], int32_t* part1_res);
static void run_program(const vector* instructions, const opcode instr_map[OPCODE_COUNT], int32_t regs[4]);
static void execute_instr(const int32_t instr[4], opcode op, const int32_t regs_in[4], int32_t regs_out[4]);
static bool is_immediate_a(opcode op);
static bool is_immediate_b(opcode op);
static void parse_input(const input* input, vector* tests, vector* instructions);

result day16_part1(const input* input)
{
	vector tests;
	parse_input(input, &tests, NULL);
	int32_t part1_res = 0;
	build_instr_map(&tests, NULL, &part1_res);

	vector_delete(&tests, NULL);
	return result_int(part1_res);
}

result day16_part2(const input* input)
{
	vector tests, instructions;
	parse_input(input, &tests, &instructions);

	opcode instr_map[OPCODE_COUNT] = { 0 };
	build_instr_map(&tests, instr_map, NULL);
	int32_t regs[4] = { 0 };
	run_program(&instructions, instr_map, regs);

	vector_delete(&tests, NULL);
	vector_delete(&instructions, NULL);
	return result_int(regs[0]);
}

void build_instr_map(const vector* tests, opcode map[OPCODE_COUNT], int32_t* part1_res)
{
	bool possible[OPCODE_COUNT][OPCODE_COUNT];
	memset(possible, true, sizeof(possible));
	for(size_t i = 0; i < tests->size; i++)
	{
		const instr_test* test = vector_at_c(tests, i);
		int32_t match_count = 0;
		for(opcode op = 0; op < OPCODE_COUNT; op++)
		{
			int32_t regs_out[4];
			memcpy(regs_out, test->inp_regs, sizeof(regs_out));
			execute_instr(test->instruction, op, test->inp_regs, regs_out);
			if(memcmp(regs_out, test->out_regs, sizeof(regs_out)) != 0)
			{
				possible[test->instruction[0]][op] = false;
			}
			else
			{
				match_count++;
			}
		}
		if(match_count >= 3 && part1_res != NULL)
		{
			(*part1_res)++;
		}
	}

	if(map != NULL)
	{
		bool changed = true;
		while(changed)
		{
			changed = false;

			for(int32_t i = 0; i < OPCODE_COUNT; i++)
			{
				int32_t possible_count = 0;
				opcode sel_code = 0;
				for(opcode code = 0; code < OPCODE_COUNT; code++)
				{
					if(possible[i][code])
					{
						possible_count++;
						sel_code = code;
					}
				}

				if(possible_count == 1)
				{
					map[i] = sel_code;
					for(int32_t k = 0; k < OPCODE_COUNT; k++)
					{
						if(i != k && possible[k][sel_code])
						{
							possible[k][sel_code] = false;
							changed = true;
						}
					}
				}
			}
		}
	}
}

void run_program(const vector* instructions, const opcode instr_map[OPCODE_COUNT], int32_t regs[4])
{
	memset(regs, 0, sizeof(int32_t) * 4);
	for(size_t i = 0; i < instructions->size; i++)
	{
		const instr* inst = vector_at_c(instructions, i);
		opcode op = instr_map[inst->code[0]];
		execute_instr(inst->code, op, regs, regs);
	}
}

void execute_instr(const int32_t instr[4], opcode op, const int32_t regs_in[4], int32_t regs_out[4])
{
	int32_t inp_a = is_immediate_a(op) ? instr[1] : regs_in[instr[1]];
	int32_t inp_b = is_immediate_b(op) ? instr[2] : regs_in[instr[2]];
	int32_t out_c = instr[3];
	switch(op)
	{
		case ADDR:
		case ADDI:
			regs_out[out_c] = inp_a + inp_b;
			break;
		case MULR:
		case MULI:
			regs_out[out_c] = inp_a * inp_b;
			break;
		case BANR:
		case BANI:
			regs_out[out_c] = inp_a & inp_b;
			break;
		case BORR:
		case BORI:
			regs_out[out_c] = inp_a | inp_b;
			break;
		case SETR:
		case SETI:
			regs_out[out_c] = inp_a;
			break;
		case GTIR:
		case GTRI:
		case GTRR:
			regs_out[out_c] = inp_a > inp_b ? 1 : 0;
			break;
		case EQIR:
		case EQRI:
		case EQRR:
			regs_out[out_c] = inp_a == inp_b ? 1 : 0;
			break;
	}
}

bool is_immediate_a(opcode op)
{
	return op == GTIR || op == EQIR || op == SETI;
}

bool is_immediate_b(opcode op)
{
	return op == ADDI || op == MULI || op == BANI || op == BORI || op == GTRI || op == EQRI;
}

void parse_input(const input* input, vector* tests, vector* instructions)
{
	size_t instructions_start = input->line_count;
	for(size_t i = 0; i < input->line_count - 3; i++)
	{
		if(input->lines_c[i].length == 0 && input->lines_c[i + 1].length == 0 && input->lines_c[i + 2].length == 0)
		{
			instructions_start = i + 3;
			break;
		}
	}

	if(tests != NULL)
	{
		*tests = vector_create(sizeof(instr_test));
		for(size_t i = 0; i < instructions_start - 3; i += 4)
		{
			instr_test test = { 0 };
			sscanf(input->lines_c[i].data, "Before: [%d, %d, %d, %d]", &test.inp_regs[0], &test.inp_regs[1], &test.inp_regs[2], &test.inp_regs[3]);
			sscanf(input->lines_c[i + 1].data, "%d %d %d %d", &test.instruction[0], &test.instruction[1], &test.instruction[2], &test.instruction[3]);
			sscanf(input->lines_c[i + 2].data, "After:  [%d, %d, %d, %d]", &test.out_regs[0], &test.out_regs[1], &test.out_regs[2], &test.out_regs[3]);
			vector_push(tests, &test);
		}
	}

	if(instructions != NULL)
	{
		*instructions = vector_create(sizeof(instr));
		for(size_t i = instructions_start; i < input->line_count; i++)
		{
			instr inst = { 0 };
			sscanf(input->lines_c[i].data, "%d %d %d %d", &inst.code[0], &inst.code[1], &inst.code[2], &inst.code[3]);
			vector_push(instructions, &inst);
		}
	}
}
