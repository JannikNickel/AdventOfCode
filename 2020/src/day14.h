#pragma once
#include "solutionbase.h"
#include <vector>
#include <utility>
#include <unordered_map>
#include <array>

struct Day14 : SolutionBase
{
	Day14(const Input& input) : SolutionBase(input, 14, "Docking Data") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Mask = std::array<int8_t, 36>;
	using MemoryAssignment = std::pair<uint64_t, uint64_t>;
	typedef void (Day14::*AssignFunc)(std::unordered_map<uint64_t, uint64_t>& memory, const MemoryAssignment& inst, const Mask& mask) const;

	struct Instruction
	{
		enum class Type { Mask, Assign } type;

		union
		{
			Mask mask;
			MemoryAssignment assign;
		};

		Instruction(Type type, const Mask& mask) : type(type), mask(mask) { }
		Instruction(Type type, const MemoryAssignment& assign) : type(type), assign(assign) { }
	};

	uint64_t CalcResult(AssignFunc assignFunc) const;
	std::unordered_map<uint64_t, uint64_t> Simulate(const std::vector<Instruction>& instructions, AssignFunc assignFunc) const;
	void AssignValueMask(std::unordered_map<uint64_t, uint64_t>& memory, const MemoryAssignment& inst, const Mask& mask) const;
	void GenVariants(Mask mask, size_t index, std::vector<Mask>& variants) const;
	void AssignAddressMask(std::unordered_map<uint64_t, uint64_t>& memory, const MemoryAssignment& inst, const Mask& mask) const;
	uint64_t ApplyMask(const Mask& mask, uint64_t value, bool changeNull = true) const;
	std::vector<Instruction> ParseInput() const;
};
