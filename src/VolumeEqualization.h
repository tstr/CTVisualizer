/*
	Volume equalization class
*/

#pragma once

#include <algorithm>

#include "Volume.h"

class Equalizer
{
public:

	Equalizer() {}

	/*
		Construct equalizer from a given volume
	*/
	Equalizer(const Volume* volume)
	{
		auto minmax = std::minmax_element(volume->constBegin(), volume->constEnd());
		m_max = *minmax.first;
		m_min = *minmax.second;
	}

	/*
		Convert 16bit voxel value to 8bit
	*/
	quint8 convert(Volume::ElementType value)
	{
		return 255 - (quint8)(255.0*((double)value - (double)m_min) / ((double)(m_max - m_min)));
	}

private:

	Volume::ElementType m_max;
	Volume::ElementType m_min;
};
