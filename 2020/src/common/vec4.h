#pragma once
#include <type_traits>

namespace common
{
	template<typename T> requires std::is_arithmetic<T>::value
	struct Vec4
	{
		T x;
		T y;
		T z;
		T w;

		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

		friend bool operator ==(const Vec4<T>& lhs, const Vec4<T>& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
		}

		friend bool operator !=(const Vec4<T>& lhs, const Vec4<T>& rhs)
		{
			return !(lhs == rhs);
		}

		Vec4<T>& operator +=(const Vec4<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		friend Vec4<T> operator +(const Vec4<T>& lhs, const Vec4<T>& rhs)
		{
			Vec4<T> res = lhs;
			res += rhs;
			return res;
		}

		Vec4<T>& operator *=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}

		friend Vec4<T> operator *(const Vec4<T>& lhs, const T& rhs)
		{
			Vec4<T> res = lhs;
			res *= rhs;
			return res;
		}

		static Vec4<T> Min(const Vec4<T>& lhs, const Vec4<T>& rhs)
		{
			return Vec4<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
		}

		static Vec4<T> Max(const Vec4<T>& lhs, const Vec4<T>& rhs)
		{
			return Vec4<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
		}
	};
}

namespace std
{
	template<typename T>
	struct hash<common::Vec4<T>>
	{
		size_t operator ()(const common::Vec4<T> value) const
		{
			size_t h = 277;
			h = h * 47 + hash<T>()(value.x);
			h = h * 47 + hash<T>()(value.y);
			h = h * 47 + hash<T>()(value.z);
			h = h * 47 + hash<T>()(value.w);
			return h;
		}
	};
}
