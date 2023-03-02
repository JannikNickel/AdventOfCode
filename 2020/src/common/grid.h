#pragma once
#include <vector>
#include <array>

namespace common
{
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

		T& At(size_t x, size_t y)
		{
			return data[y * width + x];
		}

		const T& operator ()(size_t x, size_t y) const
		{
			return data[y * width + x];
		}
	};
}
