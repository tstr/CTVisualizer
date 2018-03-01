/*
	Subimage View widget
*/

#pragma once

#include <QLabel>
#include <QVBoxLayout>

#include "gfx/VolumeRender.h"

class SubimageView : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(SubimageView)
	Q_PROPERTY(Volume::IndexType index READ index WRITE setIndex)
	Q_PROPERTY(float scale READ scale WRITE setScale RESET unsetScale)
	Q_PROPERTY(bool mip READ usesMIP WRITE useMIP)

public:
	
	/*
		Construct a subimage view widget
	*/
	explicit SubimageView(VolumeRender* render, VolumeAxis axis, Volume::IndexType index, QWidget* parent = nullptr);

	/*
		Get properties
	*/
	float scale() const { return m_scaleFactor; }
	Volume::IndexType index() const { return m_index; }
	bool usesMIP() const { return m_useMip; }

	//Reset scale
	void unsetScale() { m_scaleFactor = 1.0f; }

public slots:

	/*
		Set properties
	*/
	void setScale(float scale) { m_scaleFactor = scale; redraw(); }
	void setIndex(Volume::IndexType idx) { m_index = idx; redraw(); }
	void useMIP(bool use) { m_useMip = use; redraw(); }

	/*
		Redraw subimage
	*/
	void redraw();
	
private:

	//Input events
	void mousePressEvent(QMouseEvent* event) override;

	float m_scaleFactor = 1.0f;
	bool m_useMip = false;
	Volume::IndexType m_index = 0;

	VolumeAxis m_axis = VolumeAxis::XAxis;
	Volume::SizeType m_scaledWidth = 0;
	Volume::SizeType m_scaledHeight = 0;

	ImageBuffer m_buffer;

	QLabel m_image;
	QLabel m_imageLabel;
	QVBoxLayout m_layout;

	QString m_text;

	VolumeRender* m_render;
};
