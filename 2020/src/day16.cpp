#include "day16.h"
#include "common/stringutils.h"
#include <ranges>
#include <numeric>
#include <algorithm>
#include <unordered_set>

ResultBase* Day16::SolveFirst() const
{
	auto [valueRanges, _, otherTickets] = ParseInput();
	int valid = std::accumulate(otherTickets.begin(), otherTickets.end(), 0, [&](int acc, const Ticket& n) { return acc + CalcTicketErrorRate(n, valueRanges); });
	return new Result(valid);
}

ResultBase* Day16::SolveSecond() const
{
	auto [valueRanges, ownTicket, otherTickets] = ParseInput();
	auto it = std::remove_if(otherTickets.begin(), otherTickets.end(), [&](const Ticket& n) { return CalcTicketErrorRate(n, valueRanges) != 0; });
	otherTickets.erase(it, otherTickets.end());
	std::unordered_map<std::string, int> map = MapData(valueRanges, otherTickets);
	auto filter = map | std::views::filter([](const auto& n) { return n.first.starts_with("departure"); });
	uint64_t res = std::accumulate(filter.begin(), filter.end(), static_cast<uint64_t>(1), [&](uint64_t acc, const auto& n) { return acc * ownTicket[n.second]; });
	return new Result(res);
}

int Day16::CalcTicketErrorRate(const Ticket& ticket, const std::vector<ValueRange>& valueRanges) const
{
	int result = 0;
	for(size_t i = 0; i < ticket.size(); i++)
	{
		int value = ticket[i];
		if(std::ranges::all_of(valueRanges, [&](const ValueRange& vr) { return !(value >= vr.min0 && value <= vr.max0) && !(value >= vr.min1 && value <= vr.max1); }))
		{
			result += value;
		}
	}
	return result;
}

std::unordered_map<std::string, int> Day16::MapData(const std::vector<ValueRange>& fields, const std::vector<Ticket>& tickets) const
{
	std::unordered_map<std::string, int> map = {};
	std::unordered_set<std::string> closed = {};
	std::unordered_set<int> assignedRows = {};
	while(closed.size() != fields.size())
	{
		std::vector<std::vector<int>> possible = {};
		for(const ValueRange& field : fields)
		{
			if(closed.contains(field.name))
			{
				possible.push_back({});
				continue;
			}

			std::vector<int> possibleRows = {};
			for(size_t i = 0; i < fields.size(); i++)
			{
				if(!assignedRows.contains(i))
				{
					if(closed.size() == fields.size() - 1 || std::ranges::all_of(tickets, [&](const Ticket& ticket) { return (ticket[i] >= field.min0 && ticket[i] <= field.max0) || (ticket[i] >= field.min1 && ticket[i] <= field.max1); }))
					{
						possibleRows.push_back(i);
					}
				}
			}
			if(possibleRows.size() == 1)
			{
				closed.insert(field.name);
				map[field.name] = possibleRows[0];
				assignedRows.insert(possibleRows[0]);
				break;
			}
		}
	}
	return map;
}

Day16::Ticket Day16::ReadTicket(const std::string& line) const
{
	Ticket t = {};
	std::ranges::for_each(common::SplitStr(line, ","), [&](const std::string& n)
	{
		t.push_back(std::stoi(n));
	});
	return t;
}

std::tuple<std::vector<Day16::ValueRange>, Day16::Ticket, std::vector<Day16::Ticket>> Day16::ParseInput() const
{
	std::vector<std::string> parts = common::SplitStr(common::ReplaceStr(input.raw, "\r\n", "\n"), "\n\n");
	auto valueRanges = common::SplitStr(parts[0], "\n") | std::views::transform([](const std::string& line)
	{
		size_t start = line.find(": ") + 2;
		size_t split = line.find("-");
		size_t end = line.find(" or ");
		size_t start2 = end + 4;
		size_t split2 = line.find("-", start2);
		return ValueRange(line.substr(0, start - 2), std::stoi(line.substr(start, split)), std::stoi(line.substr(split + 1, end)), std::stoi(line.substr(start2, split2)), std::stoi(line.substr(split2 + 1)));
	});
	Ticket own = ReadTicket(common::SplitStr(parts[1], "\n")[1]);
	auto others = common::SplitStr(parts[2], "\n")
		| std::views::drop(1)
		| std::views::transform([&](const std::string& line) { return this->ReadTicket(line); });
	return std::make_tuple(std::vector<ValueRange>(valueRanges.begin(), valueRanges.end()), own, std::vector<Ticket>(others.begin(), others.end()));
}
