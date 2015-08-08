#ifndef VEC3_H
#define VEC3_H

// std includes
#include <string>
#include <cassert>

// mirage includes
#include "math.h"

namespace mirage
{

struct vec3;
extern vec3 operator+(const float f, const vec3 &other);
extern vec3 operator-(const float f, const vec3 &other);
extern vec3 operator*(const float f, const vec3 &other);
extern vec3 operator/(const float f, const vec3 &other);

struct vec3
{
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) { }

    std::string toString() const;

    bool operator==(const vec3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const vec3 &other) const
    {
        return !(*this == other);
    }

    vec3 operator+=(const vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator+=(const float f)
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    vec3 operator-=(const vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vec3 operator-=(const float f)
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    vec3 operator*=(const vec3 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    vec3 operator*=(const float f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    vec3 operator/=(const vec3 &other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    vec3 operator/=(const float f)
    {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    vec3 operator+(const vec3 &other) const
    {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator+(const float f) const
    {
        return vec3(x + f, y + f, z + f);
    }

    vec3 operator-(const vec3 &other) const
    {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator-(const float f) const
    {
        return vec3(x - f, y - f, z - f);
    }

    vec3 operator*(const vec3 &other) const
    {
        return vec3(x * other.x, y * other.y, z * other.z);
    }

    vec3 operator*(const float f) const
    {
        return vec3(x * f, y * f, z * f);
    }

    vec3 operator/(const vec3 &other) const
    {
        return vec3(x / other.x, y / other.y, z / other.z);
    }

    vec3 operator/(const float f) const
    {
        return vec3(x / f, y / f, z / f);
    }

    float operator[](int i) const
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    float &operator[](int i)
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    vec3 negate() const
    {
        return vec3(-x, -y, -z);
    }

    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec3 normalize() const
    {
        float l = 1.0f / length();
        return vec3(x * l, y * l, z * l);
    }

    static float dot(const vec3 &left, const vec3 &right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    static vec3 cross(const vec3 &left, const vec3 &right)
    {
        return vec3(left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y);
    }

    static vec3 reflect(const vec3 &I, const vec3 &N)
    {
        return I - (N * dot(N, I) * 2.0f);
    }

    static vec3 refract(const vec3 &I, const vec3 &N, float eta)
    {
        float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
        if (k < 0.0f)
            return vec3();
        else
            return eta * I - (eta * dot(N, I) + std::sqrt(k)) * N;
    }

    static vec3 sampleHemisphere(const vec3 &N)
    {
        float r1 = 2.0f * PI * pseudorand(); // Spherical coordinates
        float r2 = pseudorand();
        float r2s = std::sqrt(r2);
        vec3 w = N; // w = normal
        vec3 u = (cross((std::abs(w.x) > 0.1f ? vec3(0, 1) : vec3(1)), w)).normalize(); // u is perpendicular to w
        vec3 v = cross(w, u); // v is perpendicular to u and w
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalize();
    }

    static vec3 powv(const vec3 &v, float f)
    {
        auto x = std::pow(v.x, f);
        auto y = std::pow(v.y, f);
        auto z = std::pow(v.z, f);

        return vec3(x, y, z);
    }

    static vec3 clampv(const vec3 &v, float min, float max)
    {
        auto x = clampf(v.x, min, max);
        auto y = clampf(v.y, min, max);
        auto z = clampf(v.z, min, max);

        return vec3(x, y, z);
    }

};

inline vec3 operator+(const float f, const vec3 &other)
{
    return other + f;
}

inline vec3 operator-(const float f, const vec3 &other)
{
    return other - f;
}

inline vec3 operator*(const float f, const vec3 &other)
{
    return other * f;
}

inline vec3 operator/(const float f, const vec3 &other)
{
    return vec3(f / other.x, f / other.y, f / other.z);
}

}

#endif // VEC3_H
