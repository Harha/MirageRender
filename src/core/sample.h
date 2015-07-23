#ifndef SAMPLE_H
#define SAMPLE_H

// mirage includes
#include "../math/vec4.h"

namespace mirage
{

class Sample
{
public:
    Sample(vec4 color = vec4(), int numSamples = 0) : m_color(color), m_numSamples(numSamples) { }

    void setColor(const vec4 &color)
    {
        m_color = color;
    }

    void addColor(const vec4 &color)
    {
        m_color += color;
    }

    void decColor(const vec4 &color)
    {
        m_color -= color;
    }

    void setNumSamples(const int numSamples)
    {
        m_numSamples = numSamples;
    }

    void incNumSamples()
    {
        m_numSamples++;
    }

    void decNumSamples()
    {
        m_numSamples--;
    }

    vec4 getColor() const
    {
        return m_color;
    }

    vec4 getColorAveraged() const
    {
        return m_color / m_numSamples;
    }

    int getNumSamples() const
    {
        return m_numSamples;
    }
private:
    vec4 m_color;
    int m_numSamples;
};

}

#endif // SAMPLE_H
