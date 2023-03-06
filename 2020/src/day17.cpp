#include "day17.h"
#include <limits>

ResultBase* Day17::SolveFirst() const
{
	return new Result(Simulate(6, 3));
}

ResultBase* Day17::SolveSecond() const
{
	return new Result(Simulate(6, 4));
}

int Day17::Simulate(int steps, int dimensions) const
{
	std::unordered_set<Point> points = ParseInput();
	for(size_t i = 0; i < steps; i++)
	{
		points = Simulate(points, dimensions);
	}
	return points.size();
}

std::unordered_set<Day17::Point> Day17::Simulate(const std::unordered_set<Point>& points, int dimensions) const
{
	Point min = Point(std::numeric_limits<int>().max(), std::numeric_limits<int>().max(), std::numeric_limits<int>().max(), std::numeric_limits<int>().max());
	Point max = Point(std::numeric_limits<int>().min(), std::numeric_limits<int>().min(), std::numeric_limits<int>().min(), std::numeric_limits<int>().min());
	for(const Point& p : points)
	{
		min = Point::Min(min, p);
		max = Point::Max(max, p);
	}

	std::unordered_set<Point> next = {};
	int wDim = dimensions > 3;
	for(int x = min.x - 1; x <= max.x + 1; x++)
	{
		for(int y = min.y - 1; y <= max.y + 1; y++)
		{
			for(int z = min.z - 1; z <= max.z + 1; z++)
			{
				for(int w = min.w - wDim; w <= max.w + wDim; w++)
				{
					Point p = Point(x, y, z, w);

					int neighbours = 0;
					for(int x = -1; x <= 1; x++)
					{
						for(int y = -1; y <= 1; y++)
						{
							for(int z = -1; z <= 1; z++)
							{
								for(int w = -wDim; w <= wDim; w++)
								{
									if(x == 0 && y == 0 && z == 0 && w == 0)
									{
										continue;
									}
									neighbours += points.contains(p + Point(x, y, z, w));
								}
							}
						}
					}

					if((points.contains(p) && (neighbours == 2 || neighbours == 3)) || (!points.contains(p) && neighbours == 3))
					{
						next.insert(p);
					}
				}
			}
		}
	}
	return next;
}

std::unordered_set<Day17::Point> Day17::ParseInput() const
{
	std::unordered_set<Point> points = {};
	for(size_t i = 0; i < input.lines.size(); i++)
	{
		for(size_t k = 0; k < input.lines[i].size(); k++)
		{
			if(input.lines[i][k] == '#')
			{
				points.insert(Point(k, i, 0, 0));
			}
		}
	}
	return points;
}
