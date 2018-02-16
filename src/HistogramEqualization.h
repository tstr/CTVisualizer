/*
	Equalizer classes

	HistogramEqualizer:
		Stores/computes a histogram equalization table for a given volume

	SimpleEqualizer:
		Uses simplest equalizer method
*/

#include "Volume.h"

/*
	Histogram equalization class
*/
class HistogramEqualizer
{
public:

	/*
		Construct a histogram equalization table from a given volume
	*/
	HistogramEqualizer(const Volume*);

	/*
		Map a voxel to an 8bit value
	*/
	quint8 equalize(Volume::ElementType value) const
	{
		return m_mapping.at(value - m_min);
	}

	/*
		Get min/max values in volume
	*/
	Volume::ElementType max() const { return m_max; }
	Volume::ElementType min() const { return m_min; }

	/*
		Get mapping table
	*/
	const quint8* mapping() const { return m_mapping.data(); }

private:

	Volume::ElementType m_max;
	Volume::ElementType m_min;

	QVector<quint8> m_mapping;
};

/*
	Simple equalizer class
*/
class SimpleEqualizer
{
public:

	/*
		Construct a simple mapping from a given volume
	*/
	SimpleEqualizer(const Volume*);

	/*
		Map a voxel to an 8bit value
	*/
	quint8 equalize(Volume::ElementType value) const
	{
		return m_mapping.at(value - m_min);
	}

	/*
		Get min/max values in volume
	*/
	Volume::ElementType max() const { return m_max; }
	Volume::ElementType min() const { return m_min; }

	/*
		Get mapping table
	*/
	const quint8* mapping() const { return m_mapping.data(); }

private:

	Volume::ElementType m_max;
	Volume::ElementType m_min;

	QVector<quint8> m_mapping;
};
