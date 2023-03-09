#include "day22.h"
#include <numeric>
#include <algorithm>
#include <memory>
#include <unordered_set>

struct DequeHash
{
	size_t operator ()(const std::deque<int>& value) const
	{
		std::hash<int> hasher = {};
		size_t seed = 277;
		for(int i : value)
		{
			seed = seed * 47 + hasher(i);
		}
		return seed;
	}
};

ResultBase* Day22::SolveFirst() const
{
	return new Result(Play(false));
}

ResultBase* Day22::SolveSecond() const
{
	return new Result(Play(true));
}

uint64_t Day22::Play(bool recursive) const
{
	auto [p0, p1] = ParseInput();
	std::unique_ptr<std::deque<int>> winner = std::make_unique<std::deque<int>>();
	Game(p0, p1, recursive, winner.get());
	int mult = winner->size();
	return std::accumulate(winner->begin(), winner->end(), static_cast<uint64_t>(0), [&](int acc, int n) { return acc + n * mult--; });
}

int Day22::Game(std::deque<int> p0, std::deque<int> p1, bool recursive, std::deque<int>* winningDeck) const
{
	//std::unordered_set<std::deque<int>, DequeHash> seen = {};
	std::unordered_set<size_t> seen = {};
	while(true)
	{
		if(p0.empty() || p1.empty())
		{
			break;
		}
		/*std::deque<int> key = p0;
		key.push_back(-1);
		std::ranges::copy(p1, std::back_inserter(key));*/
		auto key = 0x3C6C84B7 * DequeHash()(p0) + DequeHash()(p1);
		if(seen.contains(key))
		{
			return 0;
		}
		seen.insert(key);

		int c0 = p0.front();
		p0.pop_front();
		int c1 = p1.front();
		p1.pop_front();
		int subWinner = (recursive && p0.size() >= c0 && p1.size() >= c1) ? Game(std::deque<int>(p0.begin(), p0.begin() + c0), std::deque<int>(p1.begin(), p1.begin() + c1), recursive) : -1;
		std::deque<int>& winner = subWinner != -1 ? (subWinner == 0 ? p0 : p1) : (c0 > c1 ? p0 : p1);
		winner.push_back(&winner == &p0 ? c0 : c1);
		winner.push_back(&winner == &p0 ? c1 : c0);
	}
	if(winningDeck != nullptr)
	{
		std::ranges::copy(p0.empty() ? p1 : p0, std::back_inserter(*winningDeck));
	}
	return p0.empty();
}

std::pair<std::deque<int>, std::deque<int>> Day22::ParseInput() const
{
	std::deque<int> players[2] = { {}, {} };
	int index = 0;
	for(const std::string& line : input.lines)
	{
		if(line.starts_with("P") || line.empty())
		{
			index += line.empty();
			continue;
		}
		players[index].push_back(std::stoi(line));
	}
	return std::make_pair(players[0], players[1]);
}
