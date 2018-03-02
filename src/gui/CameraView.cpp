/*
	3D Camera View widget
*/

#include "CameraView.h"

#include <QMouseEvent>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CameraView::CameraView(VolumeRender* render, QWidget* parent) :
	m_render(render),
	m_width(300),
	m_height(300),
	QLabel(parent)
{
	Q_ASSERT(m_render != nullptr);

	//Setup image widget
	QLabel::setAlignment(Qt::AlignCenter);
	QWidget::setFixedSize(m_width, m_height);

	//default direction
	m_viewMatrix = QMatrix4x4();
	m_viewMatrix.rotate(90, 1, 0);

	redraw();
}

/*
	Redraw event
*/
void CameraView::redraw()
{
	//Prepare buffer
	m_buffer.realloc(m_width, m_height);

	//Scaling matrix
	QMatrix4x4 scaling;
	scaling.scale(1.4f, 1.4f, 1.4f);

	//Render 3D view
	m_render->draw3D(m_buffer, m_viewMatrix * scaling);

	//Present view
	QLabel::setPixmap(m_buffer.toPixmap());
}

QVector3D CameraView::mapToSphere(const QPointF& point)
{
	float hw = (float)m_width / 2;
	float hh = (float)m_height / 2;

	//compute intersection vector
	
	const float x = point.x() - hw;
	const float y = point.y() - hh;
	const float z = sqrtf(hh*hh - x*x - y*y);

	QVector3D vec;
	vec.setX(x);
	vec.setY(y);
	//NaN occurs when point is outside sphere
	//Clamp z value to 0 if outside
	vec.setZ((std::isnan(z)) ? 0.0f : z);

	return vec.normalized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Input events
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CameraView::mousePressEvent(QMouseEvent* event)
{
	//On mouse press, update current point
	m_curPoint = mapToSphere(event->localPos());
}

void CameraView::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		//p0/p1 arcball vectors
		const QVector3D& p0 = m_curPoint;
		QVector3D p1 = mapToSphere(event->localPos());

		//Compute angle between p0 and p1
		float angle = acosf(QVector3D::dotProduct(p1, p0));
		angle *= (180.0f / 3.14159f);

		//Compute axis of rotation between p0 and p1
		QVector3D axis = QVector3D::crossProduct(p1, p0);

		//m_viewMatrix = QMatrix4x4();
		m_viewMatrix.rotate(angle, axis);

		//Update current point
		m_curPoint = p1;

		redraw();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
