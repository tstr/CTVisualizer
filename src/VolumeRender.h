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
		emit redrawAll();
	}

signals:

	void redrawAll();

private:

	void drawSubimageMIP(QImage& target, VolumeSubimageArray& viewArray);
	void drawSubimage(QImage& target, const VolumeSubimage& view);

	//Converts voxel to greyscale value
	quint8 convert(Volume::ElementType value);

	//Volume data
	Volume m_volume;
	//Volume max/min values
	Volume::ElementType m_max;
	Volume::ElementType m_min;

	//Histogram equalization
	bool m_hist = true;
	//Maximum intensity projection
	bool m_mip = false;
};
