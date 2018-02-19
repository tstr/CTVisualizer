/*
	Camera class
*/

#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:


private:

	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
};
