/*
	Volume rendering class
*/

#include "VolumeRender.h"
#include "Samplers.h"
#include "Effect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeRender::VolumeRender(Volume& volume, QObject* parent) :
	QObject(parent),
	m_volume(std::move(volume)),
	m_histogramEq(&m_volume),
	m_defaultEq(&m_volume)
{
	m_eqMapping = m_defaultEq.mapping();
}

//Converts voxel to greyscale value
quint8 VolumeRender::normalize(Volume::ElementType value)
{
	//Lookup equalized value from current mapping table
	return m_eqMapping[value - m_histogramEq.min()];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VolumeRender::drawView(QImage& target, quint32 index, VolumeAxis axis)
{
	//If maximum intensity projection enabled
	if (m_mip)
	{
		//Draw using MIP
		Effect::apply(target, [&](UV coords)
		{
			Volume::ElementType max = INT16_MIN;
			VolumeSubimageArray viewArray(&m_volume, axis);

			//Fetch maximum value of all slices at this pixel
			for (size_t k = 0; k < viewArray.length(); k++)
			{
				//Sample subimage
				const Volume::ElementType value = BilinearSampler::sample(viewArray.at(k), coords);
				//Update max value
				max = std::max(max, value);
			}

			return this->normalize(max);
		});
	}
	//Otherwise just draw a single slice
	else
	{
		VolumeSubimage view(&m_volume, index, axis);

		Effect::apply(target, [&](UV coords) {
			return this->normalize(BilinearSampler::sample(view, coords));
		});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
