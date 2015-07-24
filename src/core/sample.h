#ifndef SAMPLE_H
#define SAMPLE_H

// mirage includes
#include "../math/vec4.h"

namespace mirage
{

class Sample
{
public:
    Sample(vec4 color = vec4(), int numSamples = 0);

    void setColor(const vec4 &color);
    void addColor(const vec4 &color);
    void decColor(const vec4 &color);
    void setNumSamples(const int numSamples);
    void incNumSamples();
    void decNumSamples();
    vec4 getColor() const;
    vec4 getColorAveraged() const;
    int getNumSamples() const;
private:
    vec4 m_color;
    int m_numSamples;
};

}

#endif // SAMPLE_H
