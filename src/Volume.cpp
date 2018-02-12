/*
	Volume class implementation


	internal data representation,
	volume[slices, rows, columns]
*/

#include <QDebug>

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
	volumeData.read((char*)m_data.data(), m_data.size() * sizeof(VoxelType));

	//Compute signed minimum and maximum values
	auto minmax = minmax_element((short*)m_data.begin(), (short*)m_data.end());
	m_max = *minmax.first;
	m_min = *minmax.second;
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

//Helper function
//Convert 16bit voxel value to 8bit
Volume::EqualizedVoxelType Volume::equalize(VoxelType value) const
{
	using SignedVoxelType = typename make_signed<VoxelType>::type;

	const auto val = reinterpret_cast<const SignedVoxelType&>(value);
	//minimum and maximum
	const auto min = reinterpret_cast<const SignedVoxelType&>(m_min);
	const auto max = reinterpret_cast<const SignedVoxelType&>(m_max);

	//const uint8_t col = (uint8_t)(255.0 * ((double)val / 65553.0));
	//const uint8_t col = val / 256;
	//const uint8_t col = val;

	//for some reason the conversion formula only works with signed values
	return 255 - (EqualizedVoxelType)(255.0*((double)val - (double)min) / ((double)(max - min)));
}


//Get voxel value at point
const Volume::VoxelType& Volume::at(size_t u, size_t v, size_t w) const
{
	//verify bounds
	Q_ASSERT(u < m_columns);
	Q_ASSERT(v < m_rows);
	Q_ASSERT(w < m_slices);

	return m_data[u + m_rows * (v + m_columns * w)];
}

//Get voxel value at point
Volume::VoxelType& Volume::at(size_t u, size_t v, size_t w)
{
	//verify bounds
	Q_ASSERT(u < m_columns);
	Q_ASSERT(v < m_rows);
	Q_ASSERT(w < m_slices);

	return m_data[u + m_rows * (v + m_columns * w)];
}


QImage Volume::getY(size_t slice) const
{
	QImage img(m_columns, m_rows, QImage::Format_Grayscale8);

	for (size_t j = 0; j < img.height(); j++)
	{
		for (size_t i = 0; i < img.width(); i++)
		{
			quint8 col = equalize(at(i, j, slice));
			img.setPixel(i, j, qRgb(col, col, col));
		}
	}

	return img;
}

QImage Volume::getX(size_t row) const
{
	QImage img(m_rows, m_slices, QImage::Format_Grayscale8);

	for (size_t j = 0; j < img.height(); j++)
	{
		for (size_t i = 0; i < img.width(); i++)
		{
			quint8 col = equalize(at(i, row, j));
			img.setPixel(i, j, qRgb(col, col, col));
		}
	}

	return img;
}

QImage Volume::getZ(size_t column) const
{
	QImage img(m_columns, m_slices, QImage::Format_Grayscale8);

	for (size_t j = 0; j < img.height(); j++)
	{
		for (size_t i = 0; i < img.width(); i++)
		{
			quint8 col = equalize(at(column, i, j));
			img.setPixel(i, j, qRgb(col, col, col));
		}
	}

	return img;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
