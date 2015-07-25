#ifndef SAMPLE_H
#define SAMPLE_H

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

class Sample
{
public:
    Sample(vec3 color = vec3(), int numSamples = 0);
    void setColor(const vec3 &color);
    void addColor(const vec3 &color);
    void decColor(const vec3 &color);
    void setNumSamples(const int numSamples);
    void incNumSamples();
    void decNumSamples();
    vec3 getColor() const;
    vec3 getColorAveraged() const;
    int getNumSamples() const;
private:
    vec3 m_color;
    int m_numSamples;
};

}

#endif // SAMPLE_H
