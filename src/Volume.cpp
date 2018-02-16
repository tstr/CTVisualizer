/*
	Volume class implementation


	internal data representation,
	volume[sizeZ, sizeY, sizeX]
*/

#include <QDebug>
#include <algorithm>

#include "Volume.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

Volume::Volume(QIODevice& volumeData, size_t sizeX, size_t sizeY, size_t sizeZ) :
	m_sizeX(sizeX),
	m_sizeY(sizeY),
	m_sizeZ(sizeZ)
{
	//Reserve space in buffer
	m_data.resize((int)sizeX * sizeY * sizeZ);

	//Read directly into buffer
	volumeData.read((char*)m_data.data(), m_data.size() * sizeof(ElementType));
}

Volume::Volume(Volume&& volume)
{
	swap(volume.m_data,  this->m_data);
	swap(volume.m_sizeX, this->m_sizeX);
	swap(volume.m_sizeY, this->m_sizeY);
	swap(volume.m_sizeZ, this->m_sizeZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
