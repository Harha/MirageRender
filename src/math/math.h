#ifndef MATH_H
#define MATH_H

// std includes
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>

namespace mirage
{

// Constant definitions
#define PI 3.14159265359f
#define PI_1 1.0f / 3.14159265359f
#define EPSILON 1e-6f

// Static objects
static std::mt19937 g_randomGen(time(NULL));
static std::uniform_real_distribution<float>g_randomRealDis(0.0f, 1.0f);

// Static functions
static float pseudorand()
{
    return g_randomRealDis(g_randomGen);
}

static float clampf(float f, float min, float max)
{
    return std::max(min, std::min(f, max));
}

}

#endif // MATH_H
