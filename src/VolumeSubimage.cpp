/*
	Volume subimage source
*/

#include "VolumeSubimage.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeSubimage::VolumeSubimage(const Volume* volume, Volume::IndexType index, VolumeAxis axis) :
	m_volume(volume),
	m_index(index),
	m_axis(axis)
{
	Q_ASSERT(m_volume != nullptr);

	//Weighting values for converting (u,v,w) indices into a single index
	const Volume::SizeType xWeight = 1;
	const Volume::SizeType yWeight = volume->sizeY();
	const Volume::SizeType zWeight = volume->sizeX() * volume->sizeY();

	//Subimage is pointing along the X axis
	if (axis == VolumeAxis::XAxis)
	{
		m_width = volume->sizeY();
		m_height = volume->sizeZ();

		m_uWeight = yWeight;
		m_vWeight = zWeight;

		m_idxWeight = xWeight;
	}
	//Subimage is pointing along the Y axis
	else if (axis == VolumeAxis::YAxis)
	{
		m_width = volume->sizeX();
		m_height = volume->sizeZ();

		m_uWeight = xWeight;
		m_vWeight = zWeight;

		m_idxWeight = yWeight;
	}
	//Subimage is pointing along the Z axis
	else if (axis == VolumeAxis::ZAxis)
	{
		m_width = volume->sizeX();
		m_height = volume->sizeY();

		m_uWeight = xWeight;
		m_vWeight = yWeight;

		m_idxWeight = zWeight;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
