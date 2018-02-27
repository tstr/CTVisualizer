/*
	Volume class implementation


	internal data representation,
	volume[sizeZ, sizeY, sizeX]
*/

#include <algorithm>

#include "Volume.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

Volume::Volume(QIODevice& volumeData, const Dimensions& dimensions) :
	m_dim(dimensions)
{
	Q_ASSERT(sizeX() > 0);
	Q_ASSERT(sizeY() > 0);
	Q_ASSERT(sizeZ() > 0);

	//Reserve space in buffer
	m_data.resize(sizeX() * sizeY() * sizeZ());

	//Read directly into buffer
	volumeData.read((char*)m_data.data(), m_data.size() * sizeof(ElementType));

	//Compute minimum and maximum voxels
	auto minmax = std::minmax_element(begin(), end());
	m_min = *minmax.first;
	m_max = *minmax.second;
}

Volume::Volume(Volume&& other) :
	m_dim(other.m_dim),
	m_data(std::move(other.m_data)),
	m_min(other.m_min),
	m_max(other.m_max)
{
	other.m_dim = Dimensions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
