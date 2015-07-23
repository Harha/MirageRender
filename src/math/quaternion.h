#ifndef QUATERNION_H
#define QUATERNION_H

// std includes
#include <string>

// mirage includes
#include "math.h"
#include "vec4.h"

namespace mirage
{

struct quaternion;
extern vec4 operator*(const vec4 &v, const quaternion &q);

struct quaternion
{
    float w, x, y, z;

    quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) : w(w), x(x), y(y), z(z) { }

    std::string toString() const;

    quaternion operator*(const quaternion &q) const
    {
        quaternion result;

        result.w = w * q.w - x * q.x - y * q.y - z * q.z;
        result.x = w * q.x + x * q.w + y * q.z - z * q.y;
        result.y = w * q.y - x * q.z + y * q.w + z * q.x;
        result.z = w * q.z + x * q.y - y * q.x + z * q.w;

        return result;
    }

    quaternion operator*(const vec4 &v) const
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
        theta = 0.5f * (theta * PI / 180.0f);

        this->w = std::cos(theta);
        this->x = x * std::sin(theta);
        this->y = y * std::sin(theta);
        this->z = z * std::sin(theta);

        return *this;
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

    vec4 getRightVector() const
    {
        return vec4(1, 0, 0, 0) **this;
    }

    vec4 getUpVector() const
    {
        return vec4(0, 1, 0, 0) **this;
    }

    vec4 getForwardVector() const
    {
        return vec4(0, 0, 1, 0) **this;
    }

};

inline vec4 operator*(const vec4 &v, const quaternion &q)
{
    quaternion result;
    result = q * v * q.conjugate();
    return vec4(result.x, result.y, result.z, v.w);
}

}

#endif // QUATERNION_H
