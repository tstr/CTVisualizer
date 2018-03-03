/*
	OpenGL volume scene
*/

#pragma once

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "gfx/Volume.h"

class GLVolumeScene : public QOpenGLWidget, private QOpenGLFunctions_4_2_Core
{
	Q_OBJECT
	Q_DISABLE_COPY(GLVolumeScene)

public:

	explicit GLVolumeScene(const Volume* volume, QWidget* parent = nullptr);
	
	~GLVolumeScene();

private:

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void timerEvent(QTimerEvent* e) override;

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	bool buildShaders();
	bool buildVolume();

	//shader program
	QOpenGLShaderProgram* m_program = nullptr;
	//volume texture
	GLuint m_tex;

	//Matrices
	QMatrix4x4 m_model;
	QMatrix4x4 m_view;
	QMatrix4x4 m_proj;

	QVector3D m_curPoint;

	QStringList m_metaStrings;

	const Volume* m_volume;
};
