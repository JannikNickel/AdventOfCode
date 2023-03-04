#pragma once
#include <vector>
#include <array>
#include <functional>
#include <algorithm>
#include <array>

namespace common
{
	enum class NeighbourMode
	{
		Moore,
		VonNeumann
	};

	const std::vector<std::pair<int, int>> MOORE_OFFSETS =
	{
		std::make_pair(-1, 0),
		std::make_pair(-1, 1),
		std::make_pair(0, 1),
		std::make_pair(1, 1),
		std::make_pair(1, 0),
		std::make_pair(1, -1),
		std::make_pair(0, -1),
		std::make_pair(-1, -1)
	};

	const std::vector<std::pair<int, int>> VONNEUMANN_OFFSETS =
	{
		std::make_pair(-1, 0),
		std::make_pair(0, 1),
		std::make_pair(1, 0),
		std::make_pair(0, -1)
	};

	template<typename T>
	class Grid
	{
	private:
		size_t width;
		size_t height;
		std::vector<T> data;

	public:
		Grid(int width, int height) : width(width), height(height), data(std::vector<T>(width * height)) { }

		size_t Width() const { return width; }
		size_t Height() const { return height; }

		const std::vector<T> Data() const { return data; }

		T& At(size_t x, size_t y)
		{
			return data[y * width + x];
		}

		const T& operator ()(size_t x, size_t y) const
		{
			return data[y * width + x];
		}

		bool InBounds(size_t x, size_t y) const
		{
			return x >= 0 && x < width && y >= 0 && y < height;
		}

		int CountNeighbours(size_t x, size_t y, const std::function<bool(const T&)> pred, bool countOutOfBounds = false, NeighbourMode mode = NeighbourMode::Moore) const
		{
			const auto& offsets = (mode == NeighbourMode::Moore ? MOORE_OFFSETS : VONNEUMANN_OFFSETS);
			return std::count_if(offsets.begin(), offsets.end(), [&](const std::pair<int, int>& n)
			{
				int _x = x + n.first;
				int _y = y + n.second;
				return InBounds(_x, _y) ? pred((*this)(_x, _y)) : countOutOfBounds;
			});
		}
	};
}
