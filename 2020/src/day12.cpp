#include "day12.h"
#include "common/vec2.h"
#include <ranges>
#include <cmath>

using Vec2 = common::Vec2<int>;

ResultBase* Day12::SolveFirst() const
{
	std::vector<Action> actions = ParseInput();
	return new Result(Simulate(actions));
}

ResultBase* Day12::SolveSecond() const
{
	std::vector<Action> actions = ParseInput();
	return new Result(Simulate(actions, true));
}

Vec2 Rotate90(Vec2 source, int steps)
{
	for(int i = 0; i < std::abs(steps); i++)
	{
		source = Vec2(source.y * (steps < 0 ? -1 : 1), source.x * (steps > 0 ? -1 : 1));
	}
	return source;
}

int Day12::Simulate(const std::vector<Action>& actions, bool useWaypoint) const
{
	Vec2 pos = Vec2(0, 0);
	Vec2 dir = useWaypoint ? Vec2(10, 1) : Vec2(1, 0);
	Vec2& offsetVar = useWaypoint ? dir : pos;
	for(const Action& action : actions)
	{
		switch(action.dir)
		{
			case 'N':
				offsetVar += Vec2(0, action.value);
				break;
			case 'S':
				offsetVar += Vec2(0, -action.value);
				break;
			case 'E':
				offsetVar += Vec2(action.value, 0);
				break;
			case 'W':
				offsetVar += Vec2(-action.value, 0);
				break;
			case 'L':
				dir = Rotate90(dir, -action.value / 90);
				break;
			case 'R':
				dir = Rotate90(dir, action.value / 90);
				break;
			case 'F':
				pos += dir * action.value;
				break;
			default:
				break;
		}
	}
	return std::abs(pos.x) + std::abs(pos.y);
}

std::vector<Day12::Action> Day12::ParseInput() const
{
	auto instructions = input.lines | std::views::transform([](std::string line)
	{
		char c = line[0];
		return Action(c, std::stoi(line.substr(1)));
	});
	return std::vector<Action>(instructions.begin(), instructions.end());
}
