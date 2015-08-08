// mirage includes
#include "sample.h"

namespace mirage
{

Sample::Sample(vec3 color, int numSamples) : m_color(color), m_numSamples(numSamples)
{

}

void Sample::setColor(const vec3 &color)
{
    m_color = color;
}

void Sample::addColor(const vec3 &color)
{
    m_color = m_color + color;
    m_numSamples++;
}

void Sample::decColor(const vec3 &color)
{
    m_color = m_color - color;
    m_numSamples--;
}

void Sample::setNumSamples(const int numSamples)
{
    m_numSamples = numSamples;
}

void Sample::incNumSamples()
{
    m_numSamples++;
}

void Sample::decNumSamples()
{
    m_numSamples--;
}

void Sample::clearSample()
{
    m_color = vec3(0, 0, 0);
    m_numSamples = 0;
}

vec3 Sample::getColor() const
{
    return m_color;
}

vec3 Sample::getColorAveraged() const
{
    return m_color / m_numSamples;
}

int Sample::getNumSamples() const
{
    return m_numSamples;
}

}
