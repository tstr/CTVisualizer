/*
	Render Context class

	Provides functionality for rendering volume data.
*/

#pragma once

#include <QImage>
#include <QPixmap>
#include <QMatrix4x4>

#include "Volume.h"
#include "HistogramEqualization.h"
#include "ImageBuffer.h"

class VolumeRender : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(VolumeRender)
	//Render states
	Q_PROPERTY(bool hist READ histEnabled WRITE enableHist) // Histogram equalization

public:

	/*
		Construct a volume viewer
	*/
	explicit VolumeRender(Volume& volume, QObject* parent = nullptr);

	/*
		Draw a single subimage
	*/
	void drawSubimage(ImageBuffer& target, Volume::IndexType index, VolumeAxis axis);

	/*
		Draw an axis of the volume using Maximum Intensity Projection
	*/
	void drawSubimageMIP(ImageBuffer& target, VolumeAxis axis);

	/*
		Render the volume in 3D using the given transform
	*/
	void draw3D(ImageBuffer& target, const QMatrix4x4& modelView);

	/*
		Get volume data
	*/
	const Volume* volume() const { return &m_volume; }

	/*
		Returns true if colour mapping table is set to Histogram Equalization
	*/
	bool histEnabled() const;

public slots:

	/*
		Set the colour mapping table to Histogram Equalization
	*/
	void enableHist(bool enable);

signals:

	void redraw2D();

private:

	//Volume data
	Volume m_volume;

	//Colour mapping tables
	HistogramEqualizer m_histogramMapper;
	SimpleEqualizer m_simpleMapper;
	//Current colour mapping table
	const MappingTable* m_mapper;
};
