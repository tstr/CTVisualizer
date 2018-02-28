/*
	Camera class
*/

#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class ArcballCamera : public QObject
{
	Q_OBJECT

public:

	ArcballCamera() :
		m_xRotation(0.0f),
		m_yRotation(0.0f)
	{}

	/*
		Camera matrix
	*/
	QMatrix4x4 matrix() const
	{
		QMatrix4x4 matrix;
		matrix.rotate(m_xRotation, 1, 0, 0);
		matrix.rotate(m_yRotation, 0, 1, 0);
		//matrix.ortho(512, 512, 512, 512, 0.1f, 100.0f);
		return matrix;
	}

	float rotationX() const { return m_xRotation; }
	float rotationY() const { return m_yRotation; }

public slots:

	/*
		Rotate camera in degrees
	*/
	void setRotationX(float angle)
	{
		m_xRotation = angle;
		emit cameraUpdated(matrix());
	}

	void setRotationY(float angle)
	{
		m_yRotation = angle;
		emit cameraUpdated(matrix());
	}

signals:

	void cameraUpdated(const QMatrix4x4& matrix);

private:

	float m_xRotation;
	float m_yRotation;
};
