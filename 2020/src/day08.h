#pragma once
#include "solutionbase.h"
#include <vector>

struct Day08 : SolutionBase
{
	Day08(const Input& input) : SolutionBase(input, 8, "Handheld Halting") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	enum InstType
	{
		NOP, ACC, JMP
	};

	struct Instruction
	{
		InstType type;
		int arg;

		Instruction(InstType type, int param) : type(type), arg(param) { }
	};

	bool Simulate(const std::vector<Instruction>& instructions, int& accum) const;
	std::vector<Instruction> ParseInput() const;
};
