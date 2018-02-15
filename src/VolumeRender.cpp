/*
	Volume rendering class
*/

#include "VolumeRender.h"
#include "Samplers.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeRender::VolumeRender(Volume& volume, QObject* parent) :
	QObject(parent),
	m_volume(std::move(volume))
{
	auto minmax = std::minmax_element(m_volume.constBegin(), m_volume.constEnd());
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
	//If draw using maximum intensity projection
	if (m_mip)
	{
		//Draws all slices
		drawSubimageMIP(target, VolumeSubimageArray(&m_volume, axis));
	}
	else
	{
		//Draws single slice, specified by index
		drawSubimage(target, VolumeSubimage(&m_volume, index, axis));
	}
}

void VolumeRender::drawSubimage(QImage& target, const VolumeSubimage& view)
{
	for (size_t j = 0; j < target.height(); j++)
	{
		for (size_t i = 0; i < target.width(); i++)
		{
			const auto u = (float)i / target.width();
			const auto v = (float)j / target.height();

			const quint8 col = convert(BasicSampler::sample(view, u, v));
			target.setPixel(i, j, qRgb(col, col, col));
		}
	}
}

void VolumeRender::drawSubimageMIP(QImage& target, VolumeSubimageArray& viewArray)
{
	size_t depth = 0;


	//For each pixel in target
	for (size_t j = 0; j < target.height(); j++)
	{
		for (size_t i = 0; i < target.width(); i++)
		{
			const auto u = (float)i / target.width();
			const auto v = (float)j / target.height();

			Volume::ElementType max = INT16_MIN;

			//Fetch maximum value of all slices at this pixel
			for (size_t k = 0; k < viewArray.length(); k++)
			{
				//Sample subimage
				const Volume::ElementType value = BasicSampler::sample(viewArray.at(k), u, v);
				//Update max value
				max = std::max(max, value);
			}

			const quint8 col = convert(max);
			target.setPixel(i, j, qRgb(col, col, col));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
