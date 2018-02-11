/*
	Volume class:

	Represents a 3D volume image

	data encoded as 16 bit integer value

	 y
  	 |
	 |
	 |
	 |________x
	z
	 ___ ___ ___ ___
	|   |   |   |   |
	|___|___|___|___|
*/

#pragma once

#include <QImage>
#include <QVector>

class Volume
{
public:

	using VoxelType = ushort;

	//Trivially constructable
	Volume() {}
	//Construct volume from 3D array source
	Volume(QIODevice& volumeData, size_t columns, size_t rows, size_t slices);
	//Not copyable
	Volume(const Volume&) = delete;
	//Moveable
	Volume(Volume&& volume);

	/*
		Get dimensions of volume
	*/
	size_t getColumns() const { return m_columns; }
	size_t getRows() const { return m_rows; }
	size_t getSlices() const { return m_slices; }

	/*
		Access voxel from coordinates
	*/
	const VoxelType& at(size_t u, size_t v, size_t w) const;
	VoxelType& at(size_t u, size_t v, size_t w);

	QImage getSlice(size_t sz) const;

private:

	VoxelType m_max;
	VoxelType m_min;

	size_t m_columns = 0;
	size_t m_rows = 0;
	size_t m_slices = 0;
	
	//Data buffer
	QVector<VoxelType> m_data;
};
