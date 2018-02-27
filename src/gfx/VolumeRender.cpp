/*
	Volume rendering class
*/

#include <QElapsedTimer>

//#define NO_PARALLEL_PIXEL_FUNC

#include "VolumeRender.h"
#include "VolumeSubimageRange.h"
#include "Samplers.h"
#include "PixmapDrawer.h"

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
		return PixmapDrawer::dispatch(m_targetBuffer, [&](UV coords)
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

		return PixmapDrawer::dispatch(m_targetBuffer, [&](UV coords) {
			return normalize(BilinearSampler::sample(view, coords));
		});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap VolumeRender::draw3D(quint32 w, quint32 h, const QMatrix4x4& viewProj)
{
	//Ensure there is enough room in target buffer
	m_targetBuffer.realloc(w, h);

	return PixmapDrawer::dispatch(m_targetBuffer, [&](UV coord) {

		const QVector3D offset(0.5f, 0.5f, 0.5f);
		
		QVector4D origin(coord.toVector(), 0.0f, 1.0f);
		origin -= offset;
		origin = viewProj * origin;
		origin += offset;
		
		QVector3D dir(0, 0, 1.0f);
		dir = viewProj * dir;

		Volume::ElementType max = std::numeric_limits<Volume::ElementType>::min();

		const size_t numSteps = 120;

		for (size_t i = 0; i < numSteps; i++)
		{
			const float step = (float)i / numSteps;

			QVector4D cur = origin + (step * dir);

			const auto value = TrilinearSampler::sample(m_volume, cur.toVector3D());

			max = std::max(max, value);
		}
		
		return m_simpleMapper.normalize(max);
	});
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
