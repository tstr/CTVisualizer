/*
	Volume rendering class
*/

#include <QElapsedTimer>

//#define NO_PARALLEL_PIXEL_FUNC

#include "VolumeRender.h"
#include "VolumeSubimageRange.h"
#include "Samplers.h"
#include "ImageDrawer.h"
#include "RayCasting.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render states
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool VolumeRender::histEnabled() const
{
	return m_mapper == &m_histogramMapper;
}

void VolumeRender::enableHist(bool enable)
{
	//Change colour mapping table
	if (enable)
	{
		//Histogram equalization
		m_mapper = &m_histogramMapper;
	}
	else
	{
		//Simple normalization
		m_mapper = &m_simpleMapper;
	}

	emit redraw2D();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Drawing function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VolumeRender::drawSubimage(ImageBuffer& target, Volume::IndexType index, VolumeAxis axis)
{
	VolumeSubimage view(&m_volume, index, axis);

	ImageDrawer::dispatch(target, [&](UV coords) {
		return m_mapper->normalize(BilinearSampler::sample(view, coords));
	});
}

void VolumeRender::drawSubimageMIP(ImageBuffer& target, VolumeAxis axis)
{
	//Construct a range over the given axis
	VolumeSubimageRange range(&m_volume, axis);

	//Draw using MIP
	ImageDrawer::dispatch(target, [&](UV coords)
	{
		Volume::ElementType max = std::numeric_limits<Volume::ElementType>::min();

		//Iterate over every slice
		for (const VolumeSubimage& view : range)
		{
			//Override max if sampled value is greater
			max = std::max(max, BilinearSampler::sample(view, coords));
		}

		return m_mapper->normalize(max);
	});
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	3D
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VolumeRender::draw3D(ImageBuffer& target, const QMatrix4x4& modelView)
{
	ImageDrawer::dispatch(target, [&](UV coord)->quint8 {

		const QVector3D offset(0.5f, 0.5f, 0.5f);

		Ray ray;

		//Compute ray origin
		ray.origin = QVector4D(coord.toVector(), -1.0f, 1.0f);
		ray.origin -= offset;
		ray.origin = modelView * ray.origin;
		ray.origin += offset;

		//Compute ray direction
		ray.dir = QVector3D(0, 0, 1.0f);
		ray.dir = modelView * ray.dir;
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
