#include "day14.h"
#include <numeric>

ResultBase* Day14::SolveFirst() const
{
	return new Result(CalcResult(&Day14::AssignValueMask));
}

ResultBase* Day14::SolveSecond() const
{
	return new Result(CalcResult(&Day14::AssignAddressMask));
}

std::unordered_map<uint64_t, uint64_t> Day14::Simulate(const std::vector<Instruction>& instructions, AssignFunc assignFunc) const
{
	Mask mask;
	std::unordered_map<uint64_t, uint64_t> memory = {};
	for(const Instruction& inst : instructions)
	{
		switch(inst.type)
		{
			case Instruction::Type::Mask:
				mask = inst.mask;
				break;
			case Instruction::Type::Assign:
				(this->*assignFunc)(memory, inst.assign, mask);
				break;
			default:
				break;
		}
	}
	return memory;
}

uint64_t Day14::CalcResult(AssignFunc assignFunc) const
{
	const std::vector<Instruction> instructions = ParseInput();
	const std::unordered_map<uint64_t, uint64_t> memory = Simulate(instructions, assignFunc);
	return std::accumulate(memory.begin(), memory.end(), static_cast<uint64_t>(0), [](uint64_t acc, const auto& n)
	{
		return acc + n.second;
	});
}

void Day14::AssignValueMask(std::unordered_map<uint64_t, uint64_t>& memory, const MemoryAssignment& assign, const Mask& mask) const
{
	memory[assign.first] = ApplyMask(mask, assign.second);
}

void Day14::GenVariants(Mask mask, size_t index, std::vector<Mask>& variants) const
{
	if(index == mask.size())
	{
		variants.push_back(mask);
		return;
	}
	if(mask[index] == -1)
	{
		Mask clone = mask;
		clone[index] = 0;
		GenVariants(clone, index + 1, variants);
		clone[index] = 1;
		GenVariants(clone, index + 1, variants);
		return;
	}
	GenVariants(mask, index + 1, variants);
}

void Day14::AssignAddressMask(std::unordered_map<uint64_t, uint64_t>& memory, const MemoryAssignment& assign, const Mask& mask) const
{
	std::vector<Mask> variants = {};
	GenVariants(mask, 0, variants);
	for(Mask m : variants)
	{
		size_t a = ApplyMask(mask, assign.first, false);
		for(size_t i = 0; i < mask.size(); i++)
		{
			if(mask[i] != -1)
			{
				m[i] = -1;
			}
		}
		a = ApplyMask(m, a);
		memory[a] = assign.second;
	}
}

uint64_t Day14::ApplyMask(const Mask& mask, uint64_t value, bool changeNull) const
{
	for(size_t i = 0; i < mask.size(); i++)
	{
		if(changeNull && mask[i] == 0)
		{
			value &= ~(static_cast<uint64_t>(1) << i);
		}
		else if(mask[i] == 1)
		{
			value |= static_cast<uint64_t>(1) << i;
		}
	}
	return value;
}

std::vector<Day14::Instruction> Day14::ParseInput() const
{
	std::vector<Instruction> instructions = {};
	for(const std::string& line : input.lines)
	{
		if(line.starts_with("mask"))
		{
			std::string maskStr = line.substr(7);
			Mask mask = {};
			for(int i = maskStr.size() - 1; i >= 0; i--)
			{
				mask[35 - i] = maskStr[i] == '1' ? 1 : (maskStr[i] == '0' ? 0 : -1);
			}
			instructions.push_back(Instruction(Instruction::Type::Mask, mask));
		}
		else
		{
			MemoryAssignment memAssign = MemoryAssignment(std::stoull(line.substr(4, line.find(']') - 4)), std::stoull(line.substr(line.find("= ") + 2)));
			instructions.push_back(Instruction(Instruction::Type::Assign, memAssign));
		}
	}
	return instructions;
}
