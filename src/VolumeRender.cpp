/*
	Volume rendering class
*/

#include "VolumeRender.h"
#include "Samplers.h"
#include "Effect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeRender::VolumeRender(Volume& volume, QObject* parent) :
	QObject(parent),
	m_volume(std::move(volume))
{
	auto minmax = std::minmax_element(m_volume.begin(), m_volume.end());
	m_max = *minmax.first;
	m_min = *minmax.second;
}

//Converts voxel to greyscale value
quint8 VolumeRender::convert(Volume::ElementType value)
{
	return 255 - (quint8)(255.0*((double)value - (double)m_min) / ((double)(m_max - m_min)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VolumeRender::drawView(QImage& target, quint32 index, VolumeAxis axis)
{
	//If maximum intensity projection enabled
	if (m_mip)
	{
		//Draw using MIP
		VolumeSubimageArray viewArray(&m_volume, axis);

		Effect::apply(target, [&viewArray, this](const UV& coords)->quint8{
			Volume::ElementType max = INT16_MIN;

			//Fetch maximum value of all slices at this pixel
			for (size_t k = 0; k < viewArray.length(); k++)
			{
				//Sample subimage
				const Volume::ElementType value = BasicSampler::sample(viewArray.at(k), coords);
				//Update max value
				max = std::max(max, value);
			}

			return this->convert(max);
		});
	}
	//Otherwise just draw a single slice
	else
	{
		VolumeSubimage view(&m_volume, index, axis);

		Effect::apply(target, [&view, this](const UV& coords)->quint8 {
			return this->convert(BasicSampler::sample(view, coords));
		});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
