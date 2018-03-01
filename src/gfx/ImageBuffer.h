/*
	Image buffer class:

	Represents an 8bit grey scale image
*/

#pragma once

#include <QPixmap>
#include <QImage>

class ImageBuffer
{
public:

	using ElementType = quint8;
	using SizeType = quint32;
	using IndexType = quint32;

	ImageBuffer() {}

	/*
		Construct image buffer with reserved size
	*/
	ImageBuffer(SizeType width, SizeType height)
	{
		this->realloc(width, height);
	}

	/*
		Fetch a pixel at the given coordinates
	*/
	ElementType& at(IndexType u, IndexType v)
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);
		return m_image[(int)((v * m_width) + u)];
	}

	ElementType at(IndexType u, IndexType v) const
	{
		Q_ASSERT(u < m_width);
		Q_ASSERT(v < m_height);
		return m_image[(int)((v * m_width) + u)];
	}

	/*
		Change size of buffer.

		Old content may be discarded.
	*/
	ImageBuffer& realloc(SizeType width, SizeType height)
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
	SizeType width() const { return m_width; }
	SizeType height() const { return m_height; }

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

	SizeType m_width = 0;
	SizeType m_height = 0;
	QVector<ElementType> m_image; //data
};
