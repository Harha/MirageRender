// mirage includes
#include "mat4.h"

namespace mirage
{

std::string mat4::toString() const
{
    char buffer[255];
    snprintf(buffer, 255,
             "mat4:\n%+.2f %+.2f %+.2f %+.2f\n%+.2f %+.2f %+.2f %+.2f\n%+.2f %+.2f %+.2f %+.2f\n%+.2f %+.2f %+.2f %+.2f",
             m[0 + 0 * 4], m[0 + 1 * 4], m[0 + 2 * 4], m[0 + 3 * 4],
             m[1 + 0 * 4], m[1 + 1 * 4], m[1 + 2 * 4], m[1 + 3 * 4],
             m[2 + 0 * 4], m[2 + 1 * 4], m[2 + 2 * 4], m[2 + 3 * 4],
             m[3 + 0 * 4], m[3 + 1 * 4], m[3 + 2 * 4], m[3 + 3 * 4]);
    return std::string(buffer);
}

mat4 mat4::clear()
{
    for (size_t i = 0; i < sizeof(m) / sizeof(*m); i++)
    {
        m[i] = 0;
    }

    return *this;
}

mat4 mat4::identity()
{
    clear();

    m[0 + 0 * 4] = 1;
    m[1 + 1 * 4] = 1;
    m[2 + 2 * 4] = 1;
    m[3 + 3 * 4] = 1;

    return *this;
}

mat4 mat4::translation(const vec3 &v)
{
    identity();

    m[0 + 3 * 4] = v.x;
    m[1 + 3 * 4] = v.y;
    m[2 + 3 * 4] = v.z;

    return *this;
}

mat4 mat4::rotation(const vec3 &r, const vec3 &u, const vec3 &f)
{
    identity();

    m[0 + 0 * 4] = r.x;
    m[0 + 1 * 4] = r.y;
    m[0 + 2 * 4] = r.z;
    m[1 + 0 * 4] = u.x;
    m[1 + 1 * 4] = u.y;
    m[1 + 2 * 4] = u.z;
    m[2 + 0 * 4] = f.x;
    m[2 + 1 * 4] = f.y;
    m[2 + 2 * 4] = f.z;

    return *this;
}

mat4 mat4::scale(const vec3 &v)
{
    identity();

    m[0 + 0 * 4] = v.x;
    m[1 + 1 * 4] = v.y;
    m[2 + 2 * 4] = v.z;

    return *this;
}

mat4 mat4::orthographic(const float l, const float r, const float b, const float t, const float n, const float f)
{
    identity();

    float w = r - l;
    float h = t - b;
    float d = f - n;

    m[0 + 0 * 4] = 2.0f / w;
    m[1 + 1 * 4] = 2.0f / h;
    m[2 + 2 * 4] = -2.0f / d;
    m[0 + 3 * 4] = -(r + l) / w;
    m[1 + 3 * 4] = -(t + b) / h;
    m[2 + 3 * 4] = -(f + n) / d;

    return *this;
}

mat4 mat4::perspective(float fov, float ar, float n, float f)
{
    identity();

    float th_fov = std::tan(fov / 2.0f);
    float zrange = n - f;

    m[0 + 0 * 4] = 1.0f / (th_fov * ar);
    m[1 + 1 * 4] = 1.0f / (th_fov);
    m[2 + 2 * 4] = (-n - f) / zrange;
    m[2 + 3 * 4] = 2.0f * f * n / zrange;

    return *this;
}

}
