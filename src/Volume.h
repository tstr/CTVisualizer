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
	using Coord = size_t;

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
	Volume::ElementType at(Coord u, Coord v, Coord w) const
	{
		//verify bounds
		Q_ASSERT(u < m_columns);
		Q_ASSERT(v < m_rows);
		Q_ASSERT(w < m_slices);

		return m_data[u + m_rows * (v + m_columns * w)];
	}

	Volume::ElementType& at(Coord u, Coord v, Coord w)
	{
		//verify bounds
		Q_ASSERT(u < m_columns);
		Q_ASSERT(v < m_rows);
		Q_ASSERT(w < m_slices);

		return m_data[u + m_rows * (v + m_columns * w)];
	}

	/*
		Internal data pointer
	*/
	const ElementType* data() const { return &m_data[0]; }

	/*
		Iterators
	*/
	QVector<ElementType>::ConstIterator constBegin() const { return m_data.constBegin(); }
	QVector<ElementType>::ConstIterator constEnd() const { return m_data.constEnd(); }

private:

	ElementType m_max;
	ElementType m_min;

	size_t m_columns = 0;	//X direction
	size_t m_rows = 0;		//Y direction
	size_t m_slices = 0;	//Z direction
	
	//Data buffer
	QVector<ElementType> m_data;
};
