// mirage includes
#include "sample.h"

namespace mirage
{

Sample::Sample(vec4 color, int numSamples) : m_color(color), m_numSamples(numSamples)
{

}

void Sample::setColor(const vec4 &color)
{
    m_color = color;
}

void Sample::addColor(const vec4 &color)
{
    m_color += color;
}

void Sample::decColor(const vec4 &color)
{
    m_color -= color;
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

vec4 Sample::getColor() const
{
    return m_color;
}

vec4 Sample::getColorAveraged() const
{
    return m_color / m_numSamples;
}

int Sample::getNumSamples() const
{
    return m_numSamples;
}

}
