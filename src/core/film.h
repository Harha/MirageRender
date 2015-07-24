#ifndef FILM_H
#define FILM_H

// std includes
#include <vector>

// mirage includes
#include "sample.h"

namespace mirage
{

class Film
{
public:
    Film(int resolutionX = 128, int resolutionY = 128);

    void setSample(int x, int y, const vec4 &sample);
    void addSample(int x, int y, const vec4 &sample);
    void decSample(int x, int y, const vec4 &sample);
    int getResolutionX() const;
    int getResolutionY() const;
    float getAspectRatio() const;
    std::vector<Sample> getSamples() const;
    Sample &getSample(int x, int y);
private:
    int m_resolutionX;
    int m_resolutionY;
    float m_aspectRatio;
    std::vector<Sample> m_samples;
};

}

#endif // FILM_H
