/*
	Histogram class source
*/

#include <algorithm>

#include "HistogramEqualization.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HistogramEqualizer::HistogramEqualizer(const Volume* volume) :
	MappingTable(volume)
{
	Q_ASSERT(volume != nullptr);

	const Volume::SizeType levels = (m_volume->max() - m_volume->min()) + 1;
	const Volume::SizeType size = volume->sizeY() * volume->sizeX() * volume->sizeZ();

	//Frequency histogram
	QVector<Volume::SizeType> frequencyHistogram(levels);
	//Actual mapping table
	m_mapping.resize(levels);
	//Cumulative distribution function
	Volume::SizeType tfunction;

	//Compute frequencies of every voxel
	for (Volume::ElementType value : *volume)
	{
		frequencyHistogram[value - m_volume->min()]++;
	}

	//Initial value
	tfunction = frequencyHistogram[0];

	for (Volume::SizeType i = 1; i < levels; i++)
	{
		//Compute function for i
		tfunction += frequencyHistogram[i];
		//Compute mapping
		m_mapping[i] = (quint8)(255.0f * ((float)tfunction / size));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SimpleEqualizer::SimpleEqualizer(const Volume* volume) :
	MappingTable(volume)
{
	Q_ASSERT(volume != nullptr);

	//Value range
	const Volume::SizeType levels = (m_volume->max() - m_volume->min()) + 1;

	//Build mapping table
	m_mapping.resize(levels);

	for (Volume::SizeType i = 0; i < levels; i++)
	{
		m_mapping[i] = (quint8)(255.0f * (float)i / (levels - 1));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
