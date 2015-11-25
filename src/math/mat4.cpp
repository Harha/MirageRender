// std includes
#include <iostream>

// mirage includes
#include "mat4.h"
#include "../macros.h"

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

mat4 mat4::RGBtoXYZ()
{
    identity();

    // Adobe RGB to XYZ
    m[0 + 0 * 4] = 0.5767309f;
    m[0 + 1 * 4] = 0.1855540f;
    m[0 + 2 * 4] = 0.1881852f;

    m[1 + 0 * 4] = 0.2973769f;
    m[1 + 1 * 4] = 0.6273491f;
    m[1 + 2 * 4] = 0.0752741f;

    m[2 + 0 * 4] = 0.0270343f;
    m[2 + 1 * 4] = 0.0706872f;
    m[2 + 2 * 4] = 0.9911085f;

    return *this;
}

mat4 mat4::SRGBtoXYZ()
{
    identity();

    // Standard SRGB to XYZ
    m[0 + 0 * 4] = 0.4124564f;
    m[0 + 1 * 4] = 0.2126729f;
    m[0 + 2 * 4] = 0.0193339f;

    m[1 + 0 * 4] = 0.3575761f;
    m[1 + 1 * 4] = 0.7151522f;
    m[1 + 2 * 4] = 0.1191920f;

    m[2 + 0 * 4] = 0.1804375f;
    m[2 + 1 * 4] = 0.0721750f;
    m[2 + 2 * 4] = 0.9503041f;

    return *this;
}

mat4 mat4::transpose()
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

mat4 mat4::inverse() // 4x4 Matrix inverse, uses the numerically stable Gauss-Jordan elimination routine.
{
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
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
                        ERR("Singular matrix in mat4::inverse()");
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
            ERR("Singular matrix in mat4::inverse()");
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

}
