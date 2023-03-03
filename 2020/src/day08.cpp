#include "day08.h"
#include <ranges>

ResultBase* Day08::SolveFirst() const
{
	int accum = 0;
	Simulate(ParseInput(), accum);
	return new Result(accum);
}

ResultBase* Day08::SolveSecond() const
{
	std::vector<Instruction> instructions = ParseInput();
	for(size_t i = 0; i < instructions.size(); i++)
	{
		if(instructions[i].type != InstType::ACC)
		{
			std::vector<Instruction> mod = instructions;
			mod[i].type = mod[i].type == InstType::NOP ? InstType::JMP : InstType::NOP;
			int accum = 0;
			if(Simulate(mod, accum))
			{
				return new Result(accum);
			}
		}
	}
	return nullptr;
}

bool Day08::Simulate(const std::vector<Instruction>& instructions, int& accum) const
{
	accum = 0;
	std::vector<int> closed = std::vector<int>(instructions.size(), 0);
	int instPtr = 0;
	while(instPtr < instructions.size())
	{
		const Instruction& inst = instructions[instPtr];
		if(++closed[instPtr] > 1)
		{
			return false;
		}
		switch(inst.type)
		{
			case InstType::ACC:
				accum += inst.arg;
				break;
			case InstType::JMP:
				instPtr += inst.arg;
				continue;
			default:
				break;
		}
		instPtr += 1;
	}
	return true;
}

std::vector<Day08::Instruction> Day08::ParseInput() const
{
	auto instructions = input.lines | std::views::transform([](std::string line)
	{
		InstType type = line[0] == 'n' ? InstType::NOP : (line[0] == 'a' ? InstType::ACC : InstType::JMP);
		return Instruction(type, std::stoi(line.erase(0, 4)));
	});
	return std::vector<Day08::Instruction>(instructions.begin(), instructions.end());
}
