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
	VolumeSubimage(const Volume* volume, Volume::Index index, VolumeAxis axis);

	/*
		Subimage dimensions
	*/
	size_t height() const { return m_height; }
	size_t width() const { return m_width; }

	/*
		Index of subimage
	*/
	size_t index() const { return m_index; }

	/*
		Volume itself
	*/
	const Volume* volume() const { return m_volume; }

	/*
		Fetch a value at the given uv coordinates
	*/
	Volume::ElementType at(Volume::Index u, Volume::Index v) const
	{
		Q_ASSERT(m_volume != nullptr);
		return m_volume->data()[computeIndex(u, v)];
	}

private:

	//Compute index into volume array from Subimage uv's
	Volume::Index computeIndex(Volume::Index u, Volume::Index v) const
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);

		/*
			Formala for getting index into 3D array with 3 coordinate components:

			u + (v * rows) + (w * rows * columns)

			the index/u/v weights are set to either 1/rows/rows*columns depending on the axis
		*/
		return (m_index * m_idxWeight) + (u * m_uWeight) + (v * m_vWeight);
	}

	const Volume* m_volume = nullptr;
	VolumeAxis m_axis;
	Volume::Index m_index = 0;
	
	size_t m_width = 0;
	size_t m_height = 0;

	size_t m_uWeight;
	size_t m_vWeight;
	size_t m_idxWeight;
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
		switch (axis)
		{
			case VolumeAxis::XAxis: m_length = volume->columns();
			case VolumeAxis::YAxis: m_length = volume->rows();
			case VolumeAxis::ZAxis: m_length = volume->slices();
		}
	}

	/*
		Subimage array length
	*/
	size_t length() const { return m_length; }

	/*
		Subimage dimensions
		(constant for every subimage in array)
	*/
	size_t width() const { return m_subimage.width(); }
	size_t height() const { return m_subimage.height(); }

	/*
		Get subimage
	*/
	const VolumeSubimage& at(size_t index)
	{
		Q_ASSERT(index < m_length);
		m_subimage.m_index = index;
		return m_subimage;
	}

private:

	VolumeSubimage m_subimage;
	size_t m_length;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
