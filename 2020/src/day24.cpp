#include "day24.h"
#include <ranges>
#include <algorithm>

ResultBase* Day24::SolveFirst() const
{
	std::vector<std::vector<Dir>> tiles = ParseInput();
	return new Result(FlipInitial(tiles).size());
}

ResultBase* Day24::SolveSecond() const
{
	std::vector<std::vector<Dir>> tiles = ParseInput();
	std::unordered_set<Day24::Vec3> set = FlipInitial(tiles);
	for(size_t i = 0; i < 100; i++)
	{
		set = Flip(set);
	}
	return new Result(set.size());
}

std::unordered_set<Day24::Vec3> Day24::FlipInitial(const std::vector<std::vector<Dir>>& tiles) const
{
	std::unordered_set<Vec3> flipped = {};
	for(const std::vector<Dir>& tile : tiles)
	{
		Vec3 p = MoveTo(tile);
		if(!flipped.emplace(p).second)
		{
			flipped.erase(p);
		}
	}
	return flipped;
}

std::unordered_set<Day24::Vec3> Day24::Flip(const std::unordered_set<Vec3>& tiles) const
{
	std::unordered_set<Vec3> next = {};
	for(const Vec3& tile : tiles)
	{
		for(const Vec3 off : CUBE_OFFSETS)
		{
			const Vec3 t = tile + off;
			int neighbours = std::ranges::count_if(std::ranges::views::iota(0, 6), [&](int n) { return tiles.contains(t + CUBE_OFFSETS[n]); });
			if((tiles.contains(t) && neighbours >= 1 && neighbours <= 2) || (!tiles.contains(t) && neighbours == 2))
			{
				next.insert(t);
			}
		}
	}
	return next;
}

Day24::Vec3 Day24::MoveTo(const std::vector<Dir>& tile) const
{
	Vec3 p = Vec3(0, 0, 0);
	for(Dir dir : tile)
	{
		p += CUBE_OFFSETS[static_cast<int>(dir)];
	}
	return p;
}

Day24::Dir Day24::Next(const std::string& s, size_t& i) const
{
	switch(s[i])
	{
		case 'e':
			return Dir::E;
		case 'w':
			return Dir::W;
		case 's':
			return s[++i] == 'e' ? Dir::SE : Dir::SW;
		case 'n':
			return s[++i] == 'e' ? Dir::NE : Dir::NW;
		default:
			return Dir::NONE;
	}
}

std::vector<std::vector<Day24::Dir>> Day24::ParseInput() const
{
	std::vector<std::vector<Dir>> tiles = {};
	for(const std::string& line : input.lines)
	{
		std::vector<Dir> tile = {};
		for(size_t i = 0; i < line.size(); i++)
		{
			tile.push_back(Next(line, i));
		}
		tiles.push_back(tile);
	}
	return tiles;
}
