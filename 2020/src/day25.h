#pragma once
#include "solutionbase.h"
#include <utility>

struct Day25 : SolutionBase
{
	Day25(const Input& input) : SolutionBase(input, 25, "Combo Breaker") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	uint64_t CalcLoopSize(uint64_t key) const;
	uint64_t CalcEncryptionKey(uint64_t subjectNumber, uint64_t loopSize) const;
	std::pair<uint64_t, uint64_t> ParseInput() const;
};
