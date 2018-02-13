/*
	Volume class implementation


	internal data representation,
	volume[slices, rows, columns]
*/

#include <QDebug>
#include <algorithm>

#include "Volume.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

Volume::Volume(QIODevice& volumeData, size_t columns, size_t rows, size_t slices) :
	m_columns(columns),
	m_rows(rows),
	m_slices(slices)
{
	//Reserve space in buffer
	m_data.resize(columns * rows * slices);

	//Read directly into buffer
	volumeData.read((char*)m_data.data(), m_data.size() * sizeof(ElementType));
}

Volume::Volume(Volume&& volume)
{
	swap(volume.m_data,	   this->m_data);
	swap(volume.m_columns, this->m_columns);
	swap(volume.m_rows,    this->m_rows);
	swap(volume.m_slices,  this->m_slices);
	swap(volume.m_max,     this->m_max);
	swap(volume.m_min,     this->m_min);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Get voxel value at point
Volume::ElementType Volume::at(Coord u, Coord v, Coord w) const
{
	//verify bounds
	Q_ASSERT(u < m_columns);
	Q_ASSERT(v < m_rows);
	Q_ASSERT(w < m_slices);

	return m_data[u + m_rows * (v + m_columns * w)];
}

//Get voxel value at point
Volume::ElementType& Volume::at(Coord u, Coord v, Coord w)
{
	//verify bounds
	Q_ASSERT(u < m_columns);
	Q_ASSERT(v < m_rows);
	Q_ASSERT(w < m_slices);

	return m_data[u + m_rows * (v + m_columns * w)];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
