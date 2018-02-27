/*
	Equalizer classes

	HistogramEqualizer:
		Stores/computes a histogram equalization table for a given volume

	SimpleEqualizer:
		Uses simplest equalizer method
*/

#include "Volume.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Volume colour mapping table class
*/
class MappingTable
{
public:

	/*
		Construct an empty volume mapper
	*/
	MappingTable(const Volume* volume) :
		m_volume(volume)
	{}

	/*
		Map a voxel to an 8bit value
	*/
	quint8 normalize(Volume::ElementType value) const
	{
		return m_mapping.at(
			std::min(
				std::max(
					value - m_volume->min(),
					0
				),
				m_volume->max() - m_volume->min()
			)
		);
	}

protected:

	const Volume* m_volume;
	QVector<quint8> m_mapping;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Histogram equalization class
*/
class HistogramEqualizer : public MappingTable
{
public:

	/*
		Construct a histogram equalization table from a given volume
	*/
	HistogramEqualizer(const Volume*);
};

/*
	Simple equalizer class
*/
class SimpleEqualizer : public MappingTable
{
public:

	/*
		Construct a simple mapping from a given volume
	*/
	SimpleEqualizer(const Volume*);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
