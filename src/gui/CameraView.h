/*
	3D Camera View widget
*/

#pragma once

#include <QLabel>
#include <QVBoxLayout>
#include <QMatrix4x4>

#include "gfx/VolumeRender.h"

class CameraView : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(CameraView)

public:

	/*
		Construct a camera view widget
	*/
	explicit CameraView(VolumeRender* render, QWidget* parent = nullptr);

public slots:
	
	void redraw();
	
private:

	//Input event handlers
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	
	//Map pixel coordinates to point on arcball (result is normalized)
	QVector3D mapToSphere(const QPointF& point);

	QVector3D m_curPoint;

	//Camera matrices
	QMatrix4x4 m_viewMatrix;

	//View dimensions
	quint32 m_width;
	quint32 m_height;

	QLabel m_image;
	QVBoxLayout m_layout;

	ImageBuffer m_buffer;
	
	VolumeRender* m_render;
};
