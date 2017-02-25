// std includes
#include <cassert>

// mirage includes
#include "film.h"

namespace mirage
{

	Film::Film(int resolutionX, int resolutionY) :
		m_resolutionX(resolutionX),
		m_resolutionY(resolutionY),
		m_aspectRatio(static_cast<float>(m_resolutionX) / m_resolutionY)
	{
		m_samples.resize(m_resolutionX * m_resolutionY);
	}

	void Film::setSample(int x, int y, const vec3 & sample)
	{
		assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
		m_samples[x + y * m_resolutionX].setColor(sample);
	}

	void Film::addSample(int x, int y, const vec3 & sample)
	{
		assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
		m_samples[x + y * m_resolutionX].addColor(sample);
	}

	void Film::decSample(int x, int y, const vec3 & sample)
	{
		assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
		m_samples[x + y * m_resolutionX].decColor(sample);
	}

	void Film::clearSamples()
	{
		for (size_t i = 0; i < m_samples.size(); i++)
		{
			m_samples[i].clearSample();
		}
	}

	int Film::getResolutionX() const
	{
		return m_resolutionX;
	}

	int Film::getResolutionY() const
	{
		return m_resolutionY;
	}

	float Film::getAspectRatio() const
	{
		return m_aspectRatio;
	}

	const std::vector<Sample> & Film::getSamples() const
	{
		return m_samples;
	}

	Sample & Film::getSample(int x, int y)
	{
		assert(x >= 0 || x < m_resolutionX || y >= 0 || y < m_resolutionY);
		return m_samples[x + y * m_resolutionX];
	}

}
