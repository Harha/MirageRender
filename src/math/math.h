#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <random>
#include <ctime>

namespace mirage
{

// Constant definitions
#define PI 3.14159265359f
#define EPSILON 1e-3f

// Static objects
static std::mt19937 g_randomGen(time(NULL));
static std::uniform_real_distribution<float>g_randomRealDis(0.0f, 1.0f);

// Static functions
static float g_rng()
{
    return g_randomRealDis(g_randomGen);
}

}

#endif // MATH_H
