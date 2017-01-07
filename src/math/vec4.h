#ifndef VEC4_H
#define VEC4_H

// std includes
#include <string>

// mirage includes
#include "math.h"

namespace mirage
{

	struct vec4;
	extern vec4 operator+(const float f, const vec4 &other);
	extern vec4 operator-(const float f, const vec4 &other);
	extern vec4 operator*(const float f, const vec4 &other);
	extern vec4 operator/(const float f, const vec4 &other);

	struct vec4
	{
		float x, y, z, w;

		vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) { }

		std::string toString() const;

		inline bool operator==(const vec4 &other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		inline bool operator!=(const vec4 &other) const
		{
			return !(*this == other);
		}

		inline vec4 operator+=(const vec4 &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		inline vec4 operator+=(const float f)
		{
			x += f;
			y += f;
			z += f;
			w += f;
			return *this;
		}

		inline vec4 operator-=(const vec4 &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		inline vec4 operator-=(const float f)
		{
			x -= f;
			y -= f;
			z -= f;
			w -= f;
			return *this;
		}

		inline vec4 operator*=(const vec4 &other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		inline vec4 operator*=(const float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		inline vec4 operator/=(const vec4 &other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		inline vec4 operator/=(const float f)
		{
			float f_inv = 1.0f / f;
			x *= f_inv;
			y *= f_inv;
			z *= f_inv;
			w *= f_inv;
			return *this;
		}

		inline vec4 operator+(const vec4 &other) const
		{
			return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline vec4 operator+(const float f) const
		{
			return vec4(x + f, y + f, z + f, w + f);
		}

		inline vec4 operator-(const vec4 &other) const
		{
			return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline vec4 operator-(const float f) const
		{
			return vec4(x - f, y - f, z - f, w - f);
		}

		inline vec4 operator*(const vec4 &other) const
		{
			return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		inline vec4 operator*(const float f) const
		{
			return vec4(x * f, y * f, z * f, w * f);
		}

		inline vec4 operator/(const vec4 &other) const
		{
			return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		inline vec4 operator/(const float f) const
		{
			float f_inv = 1.0f / f;
			return vec4(x * f_inv, y * f_inv, z * f_inv, w * f_inv);
		}

		inline float operator[](int i) const
		{
			return (&x)[i];
		}

		inline float &operator[](int i)
		{
			return (&x)[i];
		}

		inline vec4 negate() const
		{
			return vec4(-x, -y, -z, -w);
		}

		inline float length() const
		{
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		inline vec4 normalize() const
		{
			float l = 1.0f / length();
			return vec4(x * l, y * l, z * l, w * l);
		}

		static inline float dot(const vec4 &left, const vec4 &right)
		{
			return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
		}

		static inline vec4 cross(const vec4 &left, const vec4 &right)
		{
			return vec4(left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y, 0.0f);
		}

		static inline vec4 reflect(const vec4 &I, const vec4 &N)
		{
			return I - (N * dot(N, I) * 2.0f);
		}

		static inline vec4 refract(const vec4 &I, const vec4 &N, float eta)
		{
			float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
			if (k < 0.0f)
				return vec4();
			else
				return eta * I - (eta * dot(N, I) + std::sqrt(k)) * N;
		}

	};

	inline vec4 operator+(const float f, const vec4 &other)
	{
		return other + f;
	}

	inline vec4 operator-(const float f, const vec4 &other)
	{
		return other - f;
	}

	inline vec4 operator*(const float f, const vec4 &other)
	{
		return other * f;
	}

	inline vec4 operator/(const float f, const vec4 &other)
	{
		return vec4(f / other.x, f / other.y, f / other.z, f / other.w);
	}

}

#endif // VEC4_H
