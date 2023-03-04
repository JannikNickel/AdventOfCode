#include "day11.h"
#include <utility>
#include <ranges>

using Point = std::pair<size_t, size_t>;

ResultBase* Day11::SolveFirst() const
{
	Grid<char> map = ParseInput();
	while(Simulate(map, 4) != 0) { }
	int occupied = std::ranges::count_if(map.Data(), [](const char& n) { return n == '#'; });
	return new Result(occupied);
	return nullptr;
}

ResultBase* Day11::SolveSecond() const
{
	Grid<char> map = ParseInput();
	while(Simulate(map, 5, true) != 0) { }
	int occupied = std::ranges::count_if(map.Data(), [](const char& n) { return n == '#'; });
	return new Result(occupied);
}

int Day11::Simulate(Grid<char>& map, int leaveThreshold, bool diagonalSearch) const
{
	std::vector<Point> pointsToSwitch = {};
	for(size_t i = 0; i < map.Width(); i++)
	{
		for(size_t k = 0; k < map.Height(); k++)
		{
			int occupied = diagonalSearch ? DiagonalSearch(map, i, k) : map.CountNeighbours(i, k, [](const char& n) { return n == '#'; });
			if((map(i, k) == 'L' && occupied == 0) || (map(i, k) == '#' && occupied >= leaveThreshold))
			{
				pointsToSwitch.push_back(Point(i, k));
			}
		}
	}
	for(const auto& [x, y] : pointsToSwitch)
	{
		map.At(x, y) = map(x, y) == 'L' ? '#' : 'L';
	}
	return pointsToSwitch.size();
}

int Day11::DiagonalSearch(const Grid<char>& map, size_t x, size_t y) const
{
	int count = 0;
	for(const auto& [xOff, yOff] : common::MOORE_OFFSETS)
	{
		int _x = x + xOff;
		int _y = y + yOff;
		while(map.InBounds(_x, _y))
		{
			if(map(_x, _y) != '.')
			{
				count += (map(_x, _y) == '#');
				break;
			}
			_x += xOff;
			_y += yOff;
		}
	}
	return count;
}

Day11::Grid<char> Day11::ParseInput() const
{
	Grid<char> grid = Grid<char>(input.lines[0].length(), input.lines.size());
	for(size_t i = 0; i < input.lines.size(); i++)
	{
		for(size_t k = 0; k < input.lines[i].length(); k++)
		{
			grid.At(k, i) = input.lines[i][k];
		}
	}
	return grid;
}
