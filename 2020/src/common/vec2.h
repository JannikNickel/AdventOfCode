#pragma once
#include <type_traits>

namespace common
{
	template<typename T> requires std::is_arithmetic<T>::value
	struct Vec2
	{
		T x;
		T y;

		Vec2(T x, T y) : x(x), y(y) { }

		friend bool operator ==(const Vec2<T>& lhs, const Vec2<T>& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}

		friend bool operator !=(const Vec2<T>& lhs, const Vec2<T>& rhs)
		{
			return !(lhs == rhs);
		}

		Vec2<T>& operator +=(const Vec2<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		friend Vec2<T> operator +(const Vec2<T>& lhs, const Vec2<T>& rhs)
		{
			Vec2<T> res = lhs;
			res += rhs;
			return res;
		}

		Vec2<T>& operator *=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		friend Vec2<T> operator *(const Vec2<T>& lhs, const T& rhs)
		{
			Vec2<T> res = lhs;
			res *= rhs;
			return res;
		}
	};
}
