/*
	Image buffer helper
*/

#pragma once

#include <QPixmap>
#include <QImage>

class ImageBuffer
{
public:

	ImageBuffer() {}

	/*
		Construct image buffer with reserved size
	*/
	ImageBuffer(size_t width, size_t height)
	{
		this->realloc(width, height);
	}

	/*
		Fetch a pixel at the given coordinates
	*/
	quint8& at(size_t u, size_t v)
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);
		return m_image[(int)((v * m_width) + u)];
	}

	quint8 at(size_t u, size_t v) const
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);
		return m_image[(int)((v * m_width) + u)];
	}

	/*
		Change size of buffer.

		Old content may be discarded.
	*/
	ImageBuffer& realloc(size_t width, size_t height)
	{
		//Increase buffer size if necessary
		if ((width * height) > (m_width * m_height))
		{
			m_image.resize((int)width * (int)height);
		}

		m_width = width;
		m_height = height;

		return *this;
	}

	/*
		Buffer dimensions
	*/
	size_t width() const { return m_width; }
	size_t height() const { return m_height; }

	/*
		Convert to image
	*/
	QImage toImage() const
	{
		return QImage(
			(const uchar*)m_image.data(),
			(int)m_width,
			(int)m_height, (int)m_width,
			QImage::Format_Grayscale8
		);
	}

	/*
		Convert to pixmap
	*/
	QPixmap toPixmap() const
	{
		return QPixmap::fromImage(toImage());
	}

private:

	size_t m_width = 0;
	size_t m_height = 0;
	QVector<quint8> m_image; //data
};
