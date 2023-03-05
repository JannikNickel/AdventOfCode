#pragma once
#include "solutionbase.h"
#include <vector>
#include <tuple>
#include <unordered_map>

struct Day16 : SolutionBase
{
	Day16(const Input& input) : SolutionBase(input, 16, "Ticket Translation") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Ticket = std::vector<int>;

	struct ValueRange
	{
		std::string name;
		int min0, max0, min1, max1;

		ValueRange(std::string name, int min0, int max0, int min1, int max1) : name(name), min0(min0), max0(max0), min1(min1), max1(max1) { }
	};

	int CalcTicketErrorRate(const Ticket& ticket, const std::vector<ValueRange>& valueRanges) const;
	std::unordered_map<std::string, int> MapData(const std::vector<ValueRange>& fields, const std::vector<Ticket>& tickets) const;
	Ticket ReadTicket(const std::string& line) const;
	std::tuple<std::vector<ValueRange>, Ticket, std::vector<Ticket>> ParseInput() const;
};
