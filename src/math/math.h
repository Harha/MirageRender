#ifndef MATH_H
#define MATH_H

// std includes
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <cstdio>

namespace mirage
{

// Constant definitions
#define PI 3.14159265359f
#define PI_2 2.0f * PI
#define PI_INV 1.0f / 3.14159265359f
#define EPSILON 1e-5f

// Static objects
static std::mt19937 g_randomGen(time(NULL));
static std::uniform_real_distribution<float>g_randomRealDis01(0.0f, 1.0f);
static std::uniform_real_distribution<float>g_randomRealDis02pi(0.0f, PI_2);

// Static functions
static float pseudorand()
{
    return g_randomRealDis01(g_randomGen);
}

static float pseudorand02pi()
{
    return g_randomRealDis02pi(g_randomGen);
}

static float clampf(float f, float min, float max)
{
    return std::max(min, std::min(f, max));
}

}

#endif // MATH_H
