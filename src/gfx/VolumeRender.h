/*
	Render Context class

	Provides functionality for viewing and volume data
*/

#pragma once

#include <QImage>
#include <QPixmap>
#include <QMatrix4x4>

#include "Volume.h"
#include "VolumeSubimage.h"
#include "HistogramEqualization.h"
#include "util/ImageBuffer.h"

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
	QPixmap drawSubimage(quint32 width, quint32 height, quint32 index, VolumeAxis axis);

	/*
		Draw a 3D view
	*/
	QPixmap draw3D(quint32 width, quint32 height, const QMatrix4x4& viewProj);

	/*
		Get volume data
	*/
	const Volume* volume() const { return &m_volume; }

	bool mipEnabled() const { return m_mip; }
	bool histEnabled() const { return m_hist; }

public slots:

	void enableMip(bool enable);
	void enableHist(bool enable);

signals:

	void redrawAll();

private:

	//Converts voxel to displayable grayscale value
	quint8 normalize(Volume::ElementType value);

	//Volume data
	Volume m_volume;

	//Temporary image buffer for performing drawing operations on
	ImageBuffer m_targetBuffer;

	//Equalizers
	HistogramEqualizer m_histogramEq;
	SimpleEqualizer m_simpleEq;
	//Current equalizer mapping table
	const quint8* m_eqMapping;

	//Histogram equalization
	bool m_hist = true;
	//Maximum intensity projection
	bool m_mip = false;
};
