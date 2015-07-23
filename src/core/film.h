#ifndef FILM_H
#define FILM_H

// std includes
#include <vector>
#include <cassert>

// mirage includes
#include "sample.h"

namespace mirage
{

class Film
{
public:
    Film(int resolutionX = 128, int resolutionY = 128) : m_resolutionX(resolutionX), m_resolutionY(resolutionY), m_aspectRatio(static_cast<float>(m_resolutionX) / m_resolutionY)
    {
        m_samples.resize(m_resolutionX * m_resolutionY);
    }

    void setSample(int x, int y, const vec4 &sample)
    {
        assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
        m_samples[x + y * m_resolutionX].setColor(sample);
    }

    void addSample(int x, int y, const vec4 &sample)
    {
        assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
        m_samples[x + y * m_resolutionX].addColor(sample);
    }

    void decSample(int x, int y, const vec4 &sample)
    {
        assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
        m_samples[x + y * m_resolutionX].decColor(sample);
    }

    int getResolutionX() const
    {
        return m_resolutionX;
    }

    int getResolutionY() const
    {
        return m_resolutionY;
    }

    float getAspectRatio() const
    {
        return m_aspectRatio;
    }

    std::vector<Sample> getSamples() const
    {
        return m_samples;
    }

    Sample &getSample(int x, int y)
    {
        assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
        return m_samples[x + y * m_resolutionX];
    }
private:
    int m_resolutionX;
    int m_resolutionY;
    float m_aspectRatio;
    std::vector<Sample> m_samples;
};

}

#endif // FILM_H
