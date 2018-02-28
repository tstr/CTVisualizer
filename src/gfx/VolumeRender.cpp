/*
	Volume rendering class
*/

#include <QElapsedTimer>

//#define NO_PARALLEL_PIXEL_FUNC

#include "VolumeRender.h"
#include "VolumeSubimageRange.h"
#include "Samplers.h"
#include "PixmapDrawer.h"
#include "RayCasting.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VolumeRender::VolumeRender(Volume& volume, QObject* parent) :
	QObject(parent),
	m_volume(std::move(volume)),
	m_histogramMapper(&m_volume),
	m_simpleMapper(&m_volume)
{
	//Set default colour mapping table
	m_mapper = &m_simpleMapper;
}

//Converts voxel to greyscale value
quint8 VolumeRender::normalize(Volume::ElementType value)
{
	//Lookup equalized value from current mapping table
	return m_mapper->normalize(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VolumeRender::enableMip(bool enable)
{
	m_mip = enable;
	emit redrawAll();
}

void VolumeRender::enableHist(bool enable)
{
	m_hist = enable;

	//Change colour mapping table
	if (m_hist)
	{
		//Histogram equalization
		m_mapper = &m_histogramMapper;
	}
	else
	{
		//Simple normalization
		m_mapper = &m_simpleMapper;
	}

	emit redrawAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap VolumeRender::drawSubimage(quint32 w, quint32 h, quint32 index, VolumeAxis axis)
{
	//Ensure there is enough room in target buffer
	m_targetBuffer.realloc(w, h);

	//If maximum intensity projection enabled
	if (m_mip)
	{
		//Construct a range over the given axis
		VolumeSubimageRange range(&m_volume, axis);

		//Draw using MIP
		PixmapDrawer::dispatch(m_targetBuffer, [&](UV coords)
		{
			Volume::ElementType max = std::numeric_limits<Volume::ElementType>::min();
			
			//Iterate over every slice
			for (const VolumeSubimage& view : range)
			{
				//Override max if sampled value is greater
				max = std::max(max, BilinearSampler::sample(view, coords));
			}

			return normalize(max);
		});
	}
	//Otherwise just draw a single slice
	else
	{
		VolumeSubimage view(&m_volume, index, axis);

		PixmapDrawer::dispatch(m_targetBuffer, [&](UV coords) {
			return normalize(BilinearSampler::sample(view, coords));
		});
	}

	return m_targetBuffer.toPixmap();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	3D
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap VolumeRender::draw3D(quint32 w, quint32 h, const QMatrix4x4& viewProj)
{
	//Ensure there is enough room in target buffer
	m_targetBuffer.realloc(w, h);

	QMatrix4x4 world;
	world.scale(1.4f, 1.4f, 1.4f);

	PixmapDrawer::dispatch(m_targetBuffer, [&](UV coord)->quint8 {

		const QVector3D offset(0.5f, 0.5f, 0.5f);

		Ray ray;

		//Compute ray origin
		ray.origin = QVector4D(coord.toVector(), -1.0f, 1.0f);
		ray.origin -= offset;
		ray.origin = viewProj * ray.origin;
		ray.origin = world * ray.origin;
		ray.origin += offset;

		//Compute ray direction
		ray.dir = QVector3D(0, 0, 1.0f);
		ray.dir = viewProj * ray.dir;
		ray.dir.normalize();

		//Default
		Volume::ElementType max = m_volume.min();

		//Perform ray cast into volume
		RaycastResult raycast = Raycast::intersects(
			AABB(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)),
			ray,
			100 //sample frequency
		);

		//Traverse volume along ray
		for (const QVector3D& pos : raycast)
		{
			//Maximum intensity projection
			max = std::max(max, TrilinearSampler::sample(m_volume, pos));
		}

		return m_simpleMapper.normalize(max);
	});

	return m_targetBuffer.toPixmap();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
