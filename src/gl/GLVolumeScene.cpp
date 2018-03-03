/*
	Volume scene
*/

#include <QFile>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include "GLVolumeScene.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLVolumeScene::GLVolumeScene(const Volume* volume, QWidget* parent) :
	QOpenGLWidget(parent),
	m_volume(volume)
{
	Q_INIT_RESOURCE(shaders);

	Q_ASSERT(QFile(":/quad.vert").exists());
	Q_ASSERT(QFile(":/volume.frag").exists());
}

GLVolumeScene::~GLVolumeScene()
{

}

bool GLVolumeScene::buildShaders()
{
	/*
		Setup shader pipeline
	*/

	m_program = new QOpenGLShaderProgram(this);

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/quad.vert")))
		return false;

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/volume.frag")))
		return false;

	if (!m_program->link())
		return false;

	if (!m_program->bind())
		return false;

	return true;
}

bool GLVolumeScene::buildVolume()
{
	//Texture dimensions
	GLsizei width = m_volume->sizeX();
	GLsizei height = m_volume->sizeY();
	GLsizei depth = m_volume->sizeZ();

	//Prepare buffer
	QVector<byte> buffer;
	buffer.resize(width * height * depth);

	//Copy volume data into normalized buffer
	for (GLsizei k = 0; k < depth; k++)
	{
		for (GLsizei j = 0; j < height; j++)
		{
			for (GLsizei i = 0; i < width; i++)
			{
				//Normalize voxel to 8 bit range
				const Volume::ElementType voxel = m_volume->at(i, j, k);
				const byte val = (byte)(255.0f * ((float)voxel - m_volume->min()) / (m_volume->max() - m_volume->min()));
				
				buffer[(k * width * height) + (j * width) + i] = val;
			}
		}
	}

	//Generate texture
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_3D, m_tex);

	/*
		Setup texture sampling parameters
	*/

	//Enable trilinear texture filtering
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Enable border address wrapping mode
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	//Set border colour
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, color);

	//Upload data
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	OpenGL function overrides
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GLVolumeScene::initializeGL()
{
	initializeOpenGLFunctions();

	if (!buildShaders())
	{
		qDebug() << m_program->log();
		close();
		return;
	}

	if (!buildVolume())
	{
		close();
		return;
	}

	m_view.rotate(90, 1, 0, 0);

	// Format info strings
	m_metaStrings += QStringLiteral("GL_VENDOR: ")                   + (const char*)glGetString(GL_VENDOR);
	m_metaStrings += QStringLiteral("GL_RENDERER: ")                 + (const char*)glGetString(GL_VERSION);
	m_metaStrings += QStringLiteral("GL_VERSION: ")                  + (const char*)glGetString(GL_VERSION);
	m_metaStrings += QStringLiteral("GL_SHADING_LANGUAGE_VERSION: ") + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void GLVolumeScene::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	//m_proj.ortho(0, w, 0, h, 0.01f, 4.0f);
}

void GLVolumeScene::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_program->bind();

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glActiveTexture(GL_TEXTURE0);
	
	m_program->setUniformValue("volume", 0);
	m_program->setUniformValue("model", m_model);
	m_program->setUniformValue("view", m_view);
	m_program->setUniformValue("proj", m_proj);

	//draw quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Draw debug text
	QPainter painter(this);
	painter.setPen(Qt::white);
	int offset = 10;
	//Draw each line
	for (const QString& str : m_metaStrings)
	{
		painter.drawText(2, offset, str);
		offset += painter.fontMetrics().height();
	}

	painter.end();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QVector3D mapToSphere(int width, int height, const QPointF& point)
{
	float hw = (float)width / 2;
	float hh = (float)height / 2;

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

void GLVolumeScene::mousePressEvent(QMouseEvent* e)
{
	//On mouse press, update current point
	m_curPoint = mapToSphere(width(), height(), e->localPos());
}

void GLVolumeScene::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		//p0/p1 arcball vectors
		const QVector3D& p0 = m_curPoint;
		QVector3D p1 = mapToSphere(width(), height(), e->localPos());

		//Compute angle between p0 and p1
		float angle = acosf(QVector3D::dotProduct(p1, p0));
		angle *= (180.0f / 3.14159f);

		//Compute axis of rotation between p0 and p1
		QVector3D axis = QVector3D::crossProduct(p1, p0);

		m_view.rotate(angle, axis);

		//Update current point
		m_curPoint = p1;

		update();
	}
}


void GLVolumeScene::mouseReleaseEvent(QMouseEvent* e)
{

}

void GLVolumeScene::timerEvent(QTimerEvent* e)
{
	qDebug() << e;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
