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

	/*
		Subimage wrapper class
	*
	class Subimage
	{
	public:

		using SampleFunc = VoxelType(*)(const Subimage*, Coord, Coord);

		//Access
		VoxelType at(Coord u, Coord v) const
		{
			Q_ASSERT(m_volume != nullptr);
			return m_func(this, u, v);
		}

		//Sub image properties
		const Volume* volume() const { return m_volume; }
		size_t height() const { return m_height; }
		size_t width() const { return m_width; }
		size_t index() const { return m_index; }

		QImage toImage() const
		{
			Q_ASSERT(m_volume != nullptr);

			QImage img(m_width, m_height, QImage::Format_Grayscale8);

			for (size_t j = 0; j < img.height(); j++)
			{
				for (size_t i = 0; i < img.width(); i++)
				{
					quint8 col = m_volume->equalize(this->at(i,j));
					img.setPixel(i, j, qRgb(col, col, col));
				}
			}

			return img;
		}

	private:

		Subimage(const Volume* volume, SampleFunc func, size_t width, size_t height, size_t index) :
			m_func(func),
			m_volume(volume),
			m_width(width),
			m_height(height),
			m_index(index)
		{}

		size_t m_width;
		size_t m_height;
		size_t m_index;

		SampleFunc m_func;
		const Volume* m_volume;

		friend class Volume;
	};
	*/

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
	ElementType at(Coord u, Coord v, Coord w) const;
	ElementType& at(Coord u, Coord v, Coord w);

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
