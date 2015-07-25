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

    mat4() : m {0, 0, 0, 0,
             0, 0, 0, 0,
             0, 0, 0, 0,
             0, 0, 0, 0
    } { }

    std::string toString() const;

    mat4 operator*(const mat4 &other) const
    {
        mat4 result;

        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                result.m[i + j * 4] = m[i + 0 * 4] * other.m[0 + j * 4] +
                                      m[i + 1 * 4] * other.m[1 + j * 4] +
                                      m[i + 2 * 4] * other.m[2 + j * 4] +
                                      m[i + 3 * 4] * other.m[3 + j * 4];
            }
        }

        return result;
    }

    vec4 operator*(const vec4 &v) const
    {
        vec4 result;

        for (size_t i = 0; i < 4; i++)
        {
            result[i] = v[0] * m[0 + i * 4] + v[1] * m[1 + i * 4] + v[2] * m[2 + i * 4] + v[3] * m[3 + i * 4];
        }

        result[0] /= result[3];
        result[1] /= result[3];
        result[2] /= result[3];

        return result;
    }

    mat4 clear();
    mat4 identity();
    mat4 translation(const vec3 &v);
    mat4 rotation(const vec3 &r, const vec3 &u, const vec3 &f);
    mat4 scale(const vec3 &v);
    mat4 orthographic(const float l, const float r, const float b, const float t, const float n, const float f);
    mat4 perspective(float fov, float ar, float n, float f);

};

}

#endif // MAT4_H
