/*
	Volume rendering class

	Provides functionality for viewing and volume data
*/

#pragma once

#include <QObject>
#include <QImage>
#include <QPixmap>

#include "Volume.h"
#include "VolumeSubimage.h"
#include "HistogramEqualization.h"

class VolumeRender : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(VolumeRender)
	Q_PROPERTY(bool mip READ mipEnabled WRITE enableMip)	// Maximum intensity projection
	Q_PROPERTY(bool hist READ histEnabled WRITE enableHist) // Histogram equalization

public:

	/*
		Construct a volume viewer
	*/
	explicit VolumeRender(Volume& volume, QObject* parent = nullptr);

	/*
		Draw a single subimage
	*/
	void drawView(QImage& target, quint32 index, VolumeAxis axis);

	/*
		Get volume data
	*/
	const Volume* volume() const { return &m_volume; }

	bool mipEnabled() const { return m_mip; }
	bool histEnabled() const { return m_hist; }

public slots:

	void enableMip(bool enable)
	{
		m_mip = enable;
		emit redrawAll();
	}

	void enableHist(bool enable)
	{
		m_hist = enable;

		//Change equalizer mapping
		if (m_hist)
		{
			m_eqMapping = m_histogramEq.mapping();
		}
		else
		{
			m_eqMapping = m_defaultEq.mapping();
		}

		emit redrawAll();
	}

signals:

	void redrawAll();

private:

	//Converts voxel to displayable grayscale value
	quint8 normalize(Volume::ElementType value);

	//Volume data
	Volume m_volume;

	//Equalizers
	HistogramEqualizer m_histogramEq;
	SimpleEqualizer m_defaultEq;
	//Current equalizer mapping table
	const quint8* m_eqMapping;

	//Histogram equalization
	bool m_hist = true;
	//Maximum intensity projection
	bool m_mip = false;
};
