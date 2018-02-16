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

	using ElementType = qint16;	//signed integer
	using IndexType = size_t;
	using SizeType = size_t;
	using Iterator = QVector<ElementType>::const_iterator;

	//Trivially constructable
	Volume() {}
	//Construct volume from 3D array source
	Volume(QIODevice& volumeData, SizeType sizeX, SizeType sizeY, SizeType sizeZ);
	//Not copyable
	Volume(const Volume&) = delete;
	//Moveable
	Volume(Volume&& volume);

	/*
		Get dimensions of volume
	*/

	//X: number of columns / width of volume
	SizeType sizeX() const { return m_sizeX; }
	//Y: number of rows / height of volume
	SizeType sizeY() const { return m_sizeY; }
	//Z: number of slices / depth of volume
	SizeType sizeZ() const { return m_sizeZ; } 

	/*
		Access voxel from coordinates
	*/
	Volume::ElementType at(IndexType u, IndexType v, IndexType w) const
	{
		//verify bounds
		Q_ASSERT(u < m_sizeX);
		Q_ASSERT(v < m_sizeY);
		Q_ASSERT(w < m_sizeZ);

		return m_data[(int)u + (int)m_sizeY * (int)(v + m_sizeX * w)];
	}

	Volume::ElementType& at(IndexType u, IndexType v, IndexType w)
	{
		//verify bounds
		Q_ASSERT(u < m_sizeX);
		Q_ASSERT(v < m_sizeY);
		Q_ASSERT(w < m_sizeZ);

		return m_data[(int)u + (int)m_sizeY * (int)(v + m_sizeX * w)];
	}

	/*
		Internal data pointer
	*/
	const ElementType* data() const { return &m_data[0]; }

	/*
		Iterators
	*/
	Iterator begin() const { return m_data.begin(); }
	Iterator end() const { return m_data.end(); }

private:

	SizeType m_sizeX = 0;
	SizeType m_sizeY = 0;
	SizeType m_sizeZ = 0;
	
	//Data buffer
	QVector<ElementType> m_data;
};
