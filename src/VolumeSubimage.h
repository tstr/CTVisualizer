/*
	Volume subimage class

	Represents a slice of a 3D volume

	Can be bound to any of the x/y/z axes
*/

#pragma once

#include "Volume.h"

class VolumeSubimageArray;

//////////////////////////////////////////////////////////////////////////////////////////////////////

enum VolumeAxis
{
	XAxis,
	YAxis,
	ZAxis
};

class VolumeSubimage
{
public:

	friend class VolumeSubimageArray;

	/*
		Construct a subimage of a volume, bound to a given index along a given axis
	*/
	VolumeSubimage(const Volume* volume, Volume::IndexType index, VolumeAxis axis);

	/*
		Subimage dimensions
	*/
	Volume::SizeType height() const { return m_height; }
	Volume::SizeType width() const { return m_width; }

	/*
		Index of subimage
	*/
	Volume::IndexType index() const { return m_index; }

	/*
		Volume itself
	*/
	const Volume* volume() const { return m_volume; }

	/*
		Fetch a value at the given uv coordinates
	*/
	Volume::ElementType at(Volume::IndexType u, Volume::IndexType v) const
	{
		Q_ASSERT(m_volume != nullptr);
		return m_volume->data()[computeIndex(u, v)];
	}

private:

	//Compute index into volume array from Subimage uv's
	inline Volume::IndexType computeIndex(Volume::IndexType u, Volume::IndexType v) const
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);

		/*
			Formala for getting index into 3D array with 3 coordinate components:

			u + (v * sizeY) + (w * sizeY * sizeX)

			the index/u/v weights are set to either 1/sizeY/sizeY*sizeX depending on the axis
		*/
		return (m_index * m_idxWeight) + (u * m_uWeight) + (v * m_vWeight);
	}

	const Volume* m_volume = nullptr;
	VolumeAxis m_axis;
	Volume::IndexType m_index = 0;
	
	Volume::SizeType m_width = 0;
	Volume::SizeType m_height = 0;

	Volume::SizeType m_uWeight;
	Volume::SizeType m_vWeight;
	Volume::SizeType m_idxWeight;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Volume subimage array class
*/
class VolumeSubimageArray
{
public:

	/*
		Construct a subimage array for a given axis
	*/
	VolumeSubimageArray(const Volume* volume, VolumeAxis axis) :
		m_subimage(VolumeSubimage(volume, 0, axis))
	{
		//Direct address table
		const Volume::SizeType lengths[] =
		{
			volume->sizeX(),
			volume->sizeY(),
			volume->sizeZ()
		};

		m_length = lengths[axis];
	}

	/*
		Subimage array length
	*/
	Volume::SizeType length() const { return m_length; }

	/*
		Subimage dimensions
		(constant for every subimage in array)
	*/
	Volume::SizeType width() const { return m_subimage.width(); }
	Volume::SizeType height() const { return m_subimage.height(); }

	/*
		Get subimage
	*/
	const VolumeSubimage& at(Volume::IndexType index)
	{
		Q_ASSERT(index < m_length);
		m_subimage.m_index = index;
		return m_subimage;
	}

private:

	VolumeSubimage m_subimage;
	Volume::SizeType m_length;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
