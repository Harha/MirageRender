#ifndef MAT4_H
#define MAT4_H

// std includes
#include <string>

// mirage includes
#include "math.h"
#include "vec3.h"
#include "vec4.h"

namespace mirage
{

	struct mat4
	{
		float m[16];

		mat4(float m[16]) : m
		{
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15],
		} { }

		mat4() : m
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		} { }

		std::string toString() const;

		inline mat4 operator*(const mat4 &other) const
		{
			mat4 result;

			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					result.m[i + j * 4] =
						m[i + 0 * 4] * other.m[0 + j * 4] +
						m[i + 1 * 4] * other.m[1 + j * 4] +
						m[i + 2 * 4] * other.m[2 + j * 4] +
						m[i + 3 * 4] * other.m[3 + j * 4];
				}
			}

			return result;
		}

		inline mat4 operator*(const float f) const
		{
			mat4 result;

			for (size_t i = 0; i < 16; i++)
			{
				result.m[i] = m[i] * f;
			}

			return result;
		}

		inline vec4 operator*(const vec4 &v) const
		{
			vec4 result;

			for (size_t i = 0; i < 4; i++)
			{
				result[i] = v[0] * m[i + 0 * 4] + v[1] * m[i + 1 * 4] + v[2] * m[i + 2 * 4] + v[3] * m[i + 3 * 4];
			}

			return result;
		}

		inline vec3 operator*(const vec3 &v) const
		{
			vec3 result;

			for (size_t i = 0; i < 3; i++)
			{
				result[i] = v[0] * m[i + 0 * 4] + v[1] * m[i + 1 * 4] + v[2] * m[i + 2 * 4];
			}

			return result;
		}

		inline float operator[](int i) const
		{
			return m[i];
		}

		inline float &operator[](int i)
		{
			return m[i];
		}

		inline mat4 clear()
		{
			for (size_t i = 0; i < sizeof(m) / sizeof(*m); i++)
			{
				m[i] = 0;
			}

			return *this;
		}

		mat4 identity();
		mat4 translation(const vec3 &v);
		mat4 rotation(const vec3 &r, const vec3 &u, const vec3 &f);
		mat4 scale(const vec3 &v);
		mat4 orthographic(const float l, const float r, const float b, const float t, const float n, const float f);
		mat4 perspective(float fov, float ar, float n, float f);
		mat4 RGBtoXYZ();
		mat4 SRGBtoXYZ();

		inline mat4 transpose()
		{
			mat4 result;

			result[0 + 0 * 4] = m[0 + 0 * 4];
			result[0 + 1 * 4] = m[1 + 0 * 4];
			result[0 + 2 * 4] = m[2 + 0 * 4];
			result[0 + 3 * 4] = m[3 + 0 * 4];

			result[1 + 0 * 4] = m[0 + 1 * 4];
			result[1 + 1 * 4] = m[1 + 1 * 4];
			result[1 + 2 * 4] = m[2 + 1 * 4];
			result[1 + 3 * 4] = m[3 + 1 * 4];

			result[2 + 0 * 4] = m[0 + 2 * 4];
			result[2 + 1 * 4] = m[1 + 2 * 4];
			result[2 + 2 * 4] = m[2 + 2 * 4];
			result[2 + 3 * 4] = m[3 + 2 * 4];

			result[3 + 0 * 4] = m[0 + 3 * 4];
			result[3 + 1 * 4] = m[1 + 3 * 4];
			result[3 + 2 * 4] = m[2 + 3 * 4];
			result[3 + 3 * 4] = m[3 + 3 * 4];

			return result;
		}

		// 4x4 Matrix inverse, uses the numerically stable Gauss-Jordan elimination routine
		inline mat4 inverse()
		{
			int indxc[4], indxr[4];
			int ipiv[4] = { 0, 0, 0, 0 };
			mat4 minv(m);

			for (int i = 0; i < 4; i++)
			{
				int irow = -1, icol = -1;
				float big = 0.f;

				// Choose pivot
				for (int j = 0; j < 4; j++)
				{
					if (ipiv[j] != 1)
					{
						for (int k = 0; k < 4; k++)
						{
							if (ipiv[k] == 0)
							{
								if (std::abs(minv[j + k * 4]) >= big)
								{
									big = std::abs(minv[j + k * 4]);
									irow = j;
									icol = k;
								}
							}
							else if (ipiv[k] > 1)
							{
								MLOG_ERROR("mat4: Singular matrix in mat4::inverse()");
							}
						}
					}
				}
				++ipiv[icol];

				// Swap rows irow and icol for pivot
				if (irow != icol)
				{
					for (int k = 0; k < 4; ++k) std::swap(minv[irow + k * 4], minv[icol + k * 4]);
				}

				indxr[i] = irow;
				indxc[i] = icol;

				if (minv[icol + icol * 4] == 0.f)
				{
					ERR("mat4: Singular matrix in mat4::inverse()");
				}

				// Set $m[icol][icol]$ to one by scaling row icol appropriately
				float pivinv = 1. / minv[icol + icol * 4];
				minv[icol + icol * 4] = 1.;
				for (int j = 0; j < 4; j++) minv[icol + j * 4] *= pivinv;

				// Subtract this row from others to zero out their columns
				for (int j = 0; j < 4; j++)
				{
					if (j != icol)
					{
						float save = minv[j + icol * 4];
						minv[j + icol * 4] = 0;
						for (int k = 0; k < 4; k++) minv[j + k * 4] -= minv[icol + k * 4] * save;
					}
				}
			}

			// Swap columns to reflect permutation
			for (int j = 3; j >= 0; j--)
			{
				if (indxr[j] != indxc[j])
				{
					for (int k = 0; k < 4; k++)
						std::swap(minv[k + indxr[j] * 4], minv[k + indxc[j] * 4]);
				}
			}

			return minv;
		}

	};

}

#endif // MAT4_H
