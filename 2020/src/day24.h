#pragma once
#include "solutionbase.h"
#include "common/vec3.h"
#include <vector>
#include <unordered_set>

struct Day24 : SolutionBase
{
	Day24(const Input& input) : SolutionBase(input, 24, "Lobby Layout") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Vec3 = common::Vec3<int>;

	enum class Dir
	{
		E, SE, SW, W, NW, NE, NONE
	};

	const Vec3 CUBE_OFFSETS[7] =
	{
		Vec3(-1, 1, 0),
		Vec3(-1, 0, 1),
		Vec3(0, -1, 1),
		Vec3(1, -1, 0),
		Vec3(1, 0, -1),
		Vec3(0, 1, -1),
		Vec3(0, 0, 0)
	};

	std::unordered_set<Vec3> FlipInitial(const std::vector<std::vector<Dir>>& tiles) const;
	std::unordered_set<Vec3> Flip(const std::unordered_set<Vec3>& tiles) const;
	Vec3 MoveTo(const std::vector<Dir>& tile) const;
	Dir Next(const std::string& s, size_t& i) const;
	std::vector<std::vector<Dir>> ParseInput() const;
};
