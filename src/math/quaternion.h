#ifndef QUATERNION_H
#define QUATERNION_H

// std includes
#include <string>

// mirage includes
#include "math.h"
#include "vec3.h"

namespace mirage
{

	struct quaternion;
	extern vec3 operator*(const vec3 &v, const quaternion &q);

	struct quaternion
	{
		float w, x, y, z;

		quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) : w(w), x(x), y(y), z(z) { }

		std::string toString() const;

		bool operator==(const quaternion &other) const
		{
			return w == other.w && x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const quaternion &other) const
		{
			return !(*this == other);
		}

		quaternion operator*(const quaternion &q) const
		{
			quaternion result;

			result.w = w * q.w - x * q.x - y * q.y - z * q.z;
			result.x = w * q.x + x * q.w + y * q.z - z * q.y;
			result.y = w * q.y - x * q.z + y * q.w + z * q.x;
			result.z = w * q.z + x * q.y - y * q.x + z * q.w;

			return result;
		}

		quaternion operator*(const vec3 &v) const
		{
			quaternion result;

			result.w = -x * v.x - y * v.y - z * v.z;
			result.x = w * v.x + y * v.z - z * v.y;
			result.y = w * v.y + z * v.x - x * v.z;
			result.z = w * v.z + x * v.y - y * v.x;

			return result;
		}

		quaternion identity()
		{
			w = 1.0f;
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;

			return *this;
		}

		quaternion euler(const float x, const float y, const float z, float theta)
		{
			theta = 0.5f * theta * PI / 180.0f;

			this->w = std::cos(theta);
			this->x = x * std::sin(theta);
			this->y = y * std::sin(theta);
			this->z = z * std::sin(theta);

			return *this;
		}

		quaternion lookAt(const vec3 &pos, const vec3 &dest)
		{
			if ((pos + dest).length() <= EPSILON)
			{
				return this->identity();
			}

			vec3 u_old = vec3(0, 1, 0);
			vec3 f_old = vec3(0, 0, 1);
			vec3 f_new = (dest - pos).normalize();
			float dot = vec3::dot(f_old, f_new);

			if (std::abs(dot + 1.0f) < EPSILON)
			{
				return this->euler(u_old.x, u_old.y, u_old.z, 180);
			}
			else if (std::abs(dot - 1.0f) < EPSILON)
			{
				return this->identity();
			}

			float theta = std::acos(dot);
			vec3 r_axs = vec3::cross(f_old, f_new).normalize();

			quaternion q = quaternion().euler(r_axs.x, r_axs.y, r_axs.z, theta * 180.0f / PI);
			//quaternion s = quaternion().fromTo(q.getUpVector(), u_old);
			return q;
		}

		quaternion fromTo(const vec3 &u, const vec3 &v)
		{
			float k = std::sqrt(vec3::dot(u, u) * vec3::dot(v, v));
			float k_cos_theta = k + vec3::dot(u, v);
			vec3 w;

			if (k_cos_theta < 1.e6f * k)
			{
				k_cos_theta = 0.0f;
				w = std::abs(u.x) > std::abs(u.z) ? vec3(-u.y, u.x, 0.f) : vec3(0.f, -u.z, u.y);
			}
			else
			{
				w = vec3::cross(u, v);
			}

			return quaternion(k_cos_theta, w.x, w.y, w.z).normalize();
		}

		quaternion conjugate() const
		{
			return quaternion(w, -x, -y, -z);
		}

		float length() const
		{
			return std::sqrt(w * w + x * x + y * y + z * z);
		}

		quaternion normalize() const
		{
			float l = 1.0f / length();
			return quaternion(w * l, x * l, y * l, z * l);
		}

		vec3 getRightVector() const
		{
			return vec3(1, 0, 0) **this;
		}

		vec3 getUpVector() const
		{
			return vec3(0, 1, 0) **this;
		}

		vec3 getForwardVector() const
		{
			return vec3(0, 0, 1) **this;
		}

	};

	inline vec3 operator*(const vec3 &v, const quaternion &q)
	{
		quaternion result;
		result = q * v * q.conjugate();
		return vec3(result.x, result.y, result.z);
	}

}

#endif // QUATERNION_H
