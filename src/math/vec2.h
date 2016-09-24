#ifndef VEC2_H
#define VEC2_H

// std includes
#include <string>

// mirage includes
#include "math.h"

namespace mirage
{

	struct vec2;
	extern vec2 operator+(const float f, const vec2 &other);
	extern vec2 operator-(const float f, const vec2 &other);
	extern vec2 operator*(const float f, const vec2 &other);
	extern vec2 operator/(const float f, const vec2 &other);

	struct vec2
	{
		float x, y;

		vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) { }

		std::string toString() const;

		bool operator==(const vec2 &other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const vec2 &other) const
		{
			return !(*this == other);
		}

		vec2 &operator+=(const vec2 &other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		vec2 &operator+=(const float f)
		{
			x += f;
			y += f;
			return *this;
		}

		vec2 &operator-=(const vec2 &other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		vec2 &operator-=(const float f)
		{
			x -= f;
			y -= f;
			return *this;
		}

		vec2 &operator*=(const vec2 &other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		vec2 &operator*=(const float f)
		{
			x *= f;
			y *= f;
			return *this;
		}

		vec2 &operator/=(const vec2 &other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		vec2 &operator/=(const float f)
		{
			float f_inv = 1.0f / f;
			x *= f_inv;
			y *= f_inv;
			return *this;
		}

		vec2 operator+(const vec2 &other) const
		{
			return vec2(x + other.x, y + other.y);
		}

		vec2 operator+(const float f) const
		{
			return vec2(x + f, y + f);
		}

		vec2 operator-(const vec2 &other) const
		{
			return vec2(x - other.x, y - other.y);
		}

		vec2 operator-(const float f) const
		{
			return vec2(x - f, y - f);
		}

		vec2 operator*(const vec2 &other) const
		{
			return vec2(x * other.x, y * other.y);
		}

		vec2 operator*(const float f) const
		{
			return vec2(x * f, y * f);
		}

		vec2 operator/(const vec2 &other) const
		{
			return vec2(x / other.x, y / other.y);
		}

		vec2 operator/(const float f) const
		{
			float f_inv = 1.0f / f;
			return vec2(x * f_inv, y * f_inv);
		}

		float operator[](int i) const
		{
			return (&x)[i];
		}

		float &operator[](int i)
		{
			return (&x)[i];
		}

		vec2 negate() const
		{
			return vec2(-x, -y);
		}

		float length() const
		{
			return std::sqrt(x * x + y * y);
		}

		vec2 normalize() const
		{
			float l = 1.0f / length();
			return vec2(x * l, y * l);
		}

		static float dot(const vec2 &left, const vec2 &right)
		{
			return left.x * right.x + left.y * right.y;
		}

		static vec2 reflect(const vec2 &I, const vec2 &N)
		{
			return I - (N * dot(N, I) * 2.0f);
		}

		static vec2 refract(const vec2 &I, const vec2 &N, float eta)
		{
			float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
			if (k < 0.0f)
				return vec2();
			else
				return eta * I - (eta * dot(N, I) + std::sqrt(k)) * N;
		}

		static vec2 powv(const vec2 &v, float f)
		{
			auto x = std::pow(v.x, f);
			auto y = std::pow(v.y, f);

			return vec2(x, y);
		}

		static vec2 clampv(const vec2 &v, float min, float max)
		{
			auto x = clampf(v.x, min, max);
			auto y = clampf(v.y, min, max);

			return vec2(x, y);
		}

	};

	inline vec2 operator+(const float f, const vec2 &other)
	{
		return other + f;
	}

	inline vec2 operator-(const float f, const vec2 &other)
	{
		return other - f;
	}

	inline vec2 operator*(const float f, const vec2 &other)
	{
		return other * f;
	}

	inline vec2 operator/(const float f, const vec2 &other)
	{
		return vec2(f / other.x, f / other.y);
	}

}

#endif // VEC2_H
