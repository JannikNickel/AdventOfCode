#pragma once
#include <ostream>
#include <string>
#include <memory>
#include "input.h"

template<class T>
concept Printable = requires(std::ostream& os, T a)
{
	os << a;
};

struct ResultBase
{
	virtual void ToStream(std::ostream& os)
	{
		os << "-";
	}
};

template<Printable T>
struct Result : ResultBase
{
	T value;

	Result(T value) : value(value) { }

	virtual void ToStream(std::ostream& os) override
	{
		os << value;
	}
};

struct SolutionBase
{
private:
	const int day;
	const std::string name;

protected:
	const Input& input;

	SolutionBase(const Input& input, int day, std::string name) : input(input), day(day), name(name) { }

	virtual ResultBase* SolveFirst() const = 0;
	virtual ResultBase* SolveSecond() const = 0;

public:
	int Day() const { return day; }
	std::string Name() const { return name; }

	std::unique_ptr<ResultBase> Solve(int part)
	{
		ResultBase* res = part == 1 ? this->SolveFirst() : this->SolveSecond();
		return std::unique_ptr<ResultBase>(res);
	}
};
