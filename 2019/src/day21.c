#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

//!(A && B && C) && D
static const char* PT1_SCRIPT =
"NOT A T\n"
"NOT T T\n"
"OR T J\n"
"NOT B T\n"
"NOT T T\n"
"AND T J\n"
"NOT C T\n"
"NOT T T\n"
"AND T J\n"
"NOT J J\n"
"AND D J\n"
"WALK\n";

//(!A || !B || (!C && H)) && D
static const char* PT2_SCRIPT =
"NOT A J\n"
"NOT B T\n"
"OR T J\n"
"NOT C T\n"
"AND H T\n"
"OR T J\n"
"AND D J\n"
"RUN\n";

int64_t run(const input* input, const char* springscript);

result day21_part1(const input* input)
{
	return result_int(run(input, PT1_SCRIPT));
}

result day21_part2(const input* input)
{
	return result_int(run(input, PT2_SCRIPT));
}

int64_t run(const input* input, const char* springscript)
{
	size_t script_len = strlen(springscript);
	int64_t* script_in = malloc(sizeof(int64_t) * script_len);
	for(size_t i = 0; i < script_len; i++)
	{
		script_in[i] = (int64_t)springscript[i];
	}
	int64_t res = intcode_run_simple_out(input->raw, script_in, script_len);
	free(script_in);
	return res;
}
