/*
	Volume subimage source
*/

#include "VolumeSubimage.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeSubimage::VolumeSubimage(const Volume* volume, Volume::Index index, VolumeAxis axis) :
	m_volume(volume),
	m_index(index),
	m_axis(axis)
{
	Q_ASSERT(m_volume != nullptr);

	//Weighting values for converting (u,v,w) indices into a single index
	const size_t xWeight = 1;
	const size_t yWeight = volume->rows();
	const size_t zWeight = volume->columns() * volume->rows();

	//Subimage is pointing along the X axis
	if (axis == VolumeAxis::XAxis)
	{
		m_width = volume->rows();
		m_height = volume->slices();

		m_uWeight = yWeight;
		m_vWeight = zWeight;

		m_idxWeight = xWeight;
	}
	//Subimage is pointing along the Y axis
	else if (axis == VolumeAxis::YAxis)
	{
		m_width = volume->columns();
		m_height = volume->slices();

		m_uWeight = xWeight;
		m_vWeight = zWeight;

		m_idxWeight = yWeight;
	}
	//Subimage is pointing along the Z axis
	else if (axis == VolumeAxis::ZAxis)
	{
		m_width = volume->columns();
		m_height = volume->rows();

		m_uWeight = xWeight;
		m_vWeight = yWeight;

		m_idxWeight = zWeight;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
