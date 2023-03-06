#pragma once
#include <type_traits>

namespace common
{
	template<typename T> requires std::is_arithmetic<T>::value
	struct Vec3
	{
		T x;
		T y;
		T z;

		Vec3(T x, T y, T z) : x(x), y(y), z(z) { }

		friend bool operator ==(const Vec3<T>& lhs, const Vec3<T>& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}

		friend bool operator !=(const Vec3<T>& lhs, const Vec3<T>& rhs)
		{
			return !(lhs == rhs);
		}

		Vec3<T>& operator +=(const Vec3<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		friend Vec3<T> operator +(const Vec3<T>& lhs, const Vec3<T>& rhs)
		{
			Vec3<T> res = lhs;
			res += rhs;
			return res;
		}

		Vec3<T>& operator *=(const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		friend Vec3<T> operator *(const Vec3<T>& lhs, const T& rhs)
		{
			Vec3<T> res = lhs;
			res *= rhs;
			return res;
		}

		static Vec3<T> Min(const Vec3<T>& lhs, const Vec3<T>& rhs)
		{
			return Vec3<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
		}

		static Vec3<T> Max(const Vec3<T>& lhs, const Vec3<T>& rhs)
		{
			return Vec3<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
		}
	};
}

namespace std
{
	template<typename T>
	struct hash<common::Vec3<T>>
	{
		size_t operator ()(const common::Vec3<T> value) const
		{
			size_t h = 277;
			h = h * 47 + hash<T>()(value.x);
			h = h * 47 + hash<T>()(value.y);
			h = h * 47 + hash<T>()(value.z);
			return h;
		}
	};
}
