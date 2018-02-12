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
*/

#pragma once

#include <QImage>
#include <QVector>

class Volume
{
public:

	using VoxelType = quint16;
	using EqualizedVoxelType = quint8;

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
	size_t columns() const { return m_columns; }
	size_t rows() const { return m_rows; }
	size_t slices() const { return m_slices; }

	/*
		Access voxel from coordinates
	*/
	const VoxelType& at(size_t u, size_t v, size_t w) const;
	VoxelType& at(size_t u, size_t v, size_t w);

	QImage getX(size_t sz) const;
	QImage getY(size_t sz) const;
	QImage getZ(size_t sz) const;

	EqualizedVoxelType equalize(VoxelType value) const;

private:

	VoxelType m_max;
	VoxelType m_min;

	size_t m_columns = 0;
	size_t m_rows = 0;
	size_t m_slices = 0;
	
	//Data buffer
	QVector<VoxelType> m_data;
};


class ImageView2D
{

};

class ImageView3D
{

};
