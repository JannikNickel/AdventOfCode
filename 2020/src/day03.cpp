#include "day03.h"
#include <initializer_list>

using namespace common;

ResultBase* Day03::SolveFirst() const
{
	Grid<int> map = ParseInput();
	int res = CountTrees(map, Vec2(0, 0), Vec2(3, 1));
	return new Result(res);
}

ResultBase* Day03::SolveSecond() const
{
	Grid<int> map = ParseInput();
	int res = 1;
	for(Vec2 step : { Vec2(1, 1), Vec2(3, 1), Vec2(5, 1), Vec2(7, 1), Vec2(1, 2) })
	{
		res *= CountTrees(map, Vec2(0, 0), step);
	}
	return new Result(res);
}

int Day03::CountTrees(const Grid<int>& map, Vec2<int> start, Vec2<int> step) const
{
	int count = 0;
	while(start.y < map.Height())
	{
		if(map(start.x, start.y) != 0)
		{
			count++;
		}
		start += step;
		start.x %= map.Width();
	}
	return count;
}

Grid<int> Day03::ParseInput() const
{
	Grid<int> grid = Grid<int>(input.lines[0].length(), input.lines.size());
	for(size_t i = 0; i < input.lines.size(); i++)
	{
		for(size_t k = 0; k < input.lines[i].length(); k++)
		{
			grid.At(k, i) = input.lines[i][k] == '#';
		}
	}
	return grid;
}
