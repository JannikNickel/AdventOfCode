#ifndef CHRONAL_DEVICE_H
#define CHRONAL_DEVICE_H

#include "common.h"

typedef struct
{
	char op[4];
	int32_t a;
	int32_t b;
	int32_t c;
} instr;

void chronal_execute_instr(const instr* instruction, int32_t ip_reg, int32_t regs[6]);
bool chronal_is_immediate_a(const char op[4]);
bool chronal_is_immediate_b(const char op[4]);
vector chronal_parse_input(const input* input, int32_t* ip);

#endif
