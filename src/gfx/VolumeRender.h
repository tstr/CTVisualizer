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
#include "Samplers.h"

enum SamplingType
{
	SamplingBasic, //nearest-neighbour
	SamplingBilinear,
	SamplingBicubic
};

class VolumeRender : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(VolumeRender)
	//Render states
	Q_PROPERTY(bool hist READ histEnabled WRITE enableHist) // Histogram equalization
	Q_PROPERTY(SamplingType sampling READ getSamplingType WRITE setSamplingType)
	Q_PROPERTY(quint32 sampleFrequency READ getSampleFrequency WRITE setSampleFrequency)

public:

	//////////////////////////////////////////////////////////////////////////////////

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
		Properties
	*/

	//Returns true if colour mapping table is set to Histogram Equalization
	bool histEnabled() const;
	//Return the sampling type
	SamplingType getSamplingType() const;
	//Return the raycast sample frequency
	quint32 getSampleFrequency() const { return m_sampleFrequency; }

public slots:

	//Set the colour mapping table to Histogram Equalization
	void enableHist(bool enable);

	//Set the sampling type
	void setSamplingType(SamplingType type);
	//Enable specific sampling types
	void setSamplingTypeBasic() { setSamplingType(SamplingBasic); }
	void setSamplingTypeBilinear() { setSamplingType(SamplingBilinear); }
	void setSamplingTypeBicubic() { setSamplingType(SamplingBicubic); }

	//Set the raycast sampling frequency
	void setSampleFrequency(quint32 frequency) { m_sampleFrequency = frequency; redraw3D(); }

signals:

	void redraw2D();
	void redraw3D();

private:

	//Volume data
	Volume m_volume;

	//Sampling function
	SamplerFunc2D m_samplerFunc;

	//Raycast sample frequency
	quint32 m_sampleFrequency;

	//Colour mapping tables
	HistogramEqualizer m_histogramMapper;
	SimpleEqualizer m_simpleMapper;

	//Current colour mapping table
	const MappingTable* m_mapper;
};
