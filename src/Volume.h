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
	using IndexType = quint32;
	using SizeType = quint32;
	using Iterator = QVector<ElementType>::const_iterator;

	/*
		Volume dimensions description structure
	*/
	struct Dimensions
	{
		//Size in voxels
		SizeType sizeX = 0;
		SizeType sizeY = 0;
		SizeType sizeZ = 0;

		//Voxel scale factor
		SizeType scaleX = 1;
		SizeType scaleY = 1;
		SizeType scaleZ = 1;

		/*
			Constructors
		*/
		Dimensions() {}
		Dimensions(const Dimensions& other) = default;
		Dimensions(SizeType _sizeX, SizeType _sizeY, SizeType _sizeZ, SizeType _scaleX, SizeType _scaleY, SizeType _scaleZ) :
			sizeX(_sizeX), sizeY(_sizeY), sizeZ(sizeZ),
			scaleX(_scaleX), scaleY(_scaleY), scaleZ(_scaleZ)
		{}
	};

	//Trivially constructable
	Volume() {}
	//Construct volume from 3D array source
	Volume(QIODevice& volumeData, const Dimensions& dimensions);
	//Not copyable
	Volume(const Volume&) = delete;
	//Moveable
	Volume(Volume&& volume);

	/*
		Get dimensions of volume
	*/

	//X: number of columns / width of volume
	SizeType sizeX() const { return m_dim.sizeX; }
	//Y: number of rows / height of volume
	SizeType sizeY() const { return m_dim.sizeY; }
	//Z: number of slices / depth of volume
	SizeType sizeZ() const { return m_dim.sizeZ; }

	//X: Voxel factor
	SizeType scaleX() const { return m_dim.scaleX; }
	//Y: Voxel factor
	SizeType scaleY() const { return m_dim.scaleY; }
	//Z: Voxel factor
	SizeType scaleZ() const { return m_dim.scaleZ; }

	/*
		Access voxel from coordinates
	*/
	Volume::ElementType at(IndexType u, IndexType v, IndexType w) const
	{
		//verify bounds
		Q_ASSERT(u < sizeX());
		Q_ASSERT(v < sizeY());
		Q_ASSERT(w < sizeZ());

		return m_data[u + sizeY() * (v + sizeX() * w)];
	}

	Volume::ElementType& at(IndexType u, IndexType v, IndexType w)
	{
		//verify bounds
		Q_ASSERT(u < sizeX());
		Q_ASSERT(v < sizeY());
		Q_ASSERT(w < sizeZ());

		return m_data[u + sizeY() * (v + sizeX() * w)];
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

	//Volume dimension info
	Dimensions m_dim;

	//Data buffer
	QVector<ElementType> m_data;
};
