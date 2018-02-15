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
	m_data.resize((int)columns * rows * slices);

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
