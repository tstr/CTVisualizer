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
	m_histogramEq(&m_volume),
	m_simpleEq(&m_volume)
{
	m_eqMapping = m_simpleEq.mapping();
}

//Converts voxel to greyscale value
quint8 VolumeRender::normalize(Volume::ElementType value)
{
	//Lookup equalized value from current mapping table
	return m_eqMapping[value - m_histogramEq.min()];
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

	//Change equalizer mapping
	if (m_hist)
	{
		m_eqMapping = m_histogramEq.mapping();
	}
	else
	{
		m_eqMapping = m_simpleEq.mapping();
	}

	emit redrawAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap VolumeRender::drawSubimage(quint32 w, quint32 h, quint32 index, VolumeAxis axis)
{
	//If maximum intensity projection enabled
	if (m_mip)
	{
		//Construct a range over the given axis
		VolumeSubimageRange range(&m_volume, axis);

		//Draw using MIP
		return PixmapDrawer::dispatch(w, h, [&](UV coords)
		{
			Volume::ElementType max = std::numeric_limits<Volume::ElementType>::min();
			
			//Iterate over every slice
			for (const VolumeSubimage& view : range)
			{
				//Override max if sampled value is greater
				max = std::max(max, BilinearSampler::sample(view, coords));
			}

			return this->normalize(max);
		});
	}
	//Otherwise just draw a single slice
	else
	{
		VolumeSubimage view(&m_volume, index, axis);

		return PixmapDrawer::dispatch(w, h, [&](UV coords) {
			return this->normalize(BilinearSampler::sample(view, coords));
		});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap VolumeRender::draw3D(quint32 w, quint32 h, const QMatrix4x4& viewProj)
{
	return PixmapDrawer::dispatch(w, h, [&](UV coord) {
		const QVector3D offset(0.5f, 0.5f, 0.5f);

		QVector4D origin(coord.u, coord.v, 0.0f, 1.0f);
		origin -= offset;
		origin = viewProj * origin;
		origin += offset;
		
		QVector3D dir(0, 0, 1.0f);
		dir = viewProj * dir;

		Volume::ElementType max = std::numeric_limits<Volume::ElementType>::min();

		const size_t numSteps = 100;

		for (size_t i = 0; i < numSteps; i++)
		{
			const float step = (float)i / numSteps;

			QVector4D cur = origin + (step * dir);

			const auto value = TrilinearSampler::sample(m_volume, UVW(cur.toVector3D()));

			max = std::max(max, value);
		}

		return normalize(max);
	});
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
