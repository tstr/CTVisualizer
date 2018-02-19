/*
	Pixmap drawing class for filling pixmaps procedurally
*/
#pragma once

#include <QImage>
#include <QtConcurrentMap>

#include "util/CountingIterator.h"
#include <QDebug>

class PixmapDrawer
{
public:

	/*
		Construct a Pixmap of size w x h where each pixel is computed from a given pixel function.

		Pixel function must take a UV as an argument and return a byte value
	*/
	template<typename PixelFunc>
	static QPixmap dispatch(quint32 width, quint32 height, const PixelFunc& pixel)
	{
		QImage target(width, height, QImage::Format_Grayscale8);

		//Per-pixel procedure
		auto proc = [&](size_t n) {

			const size_t i = n % width;
			const size_t j = n / width;

			//Relative texture coordinates
			const auto u = (float)i / width;
			const auto v = (float)j / height;

			//Apply function
			const quint8 col = pixel(UV(u, v));

			//Store result
			target.setPixel((int)i, (int)j, qRgb((int)col, (int)col, (int)col));
		};

#ifdef NO_PARALLEL_PIXEL_FUNC

		//Sequential foreach
		for (size_t i = 0; i < (height * width); i++)
		{
			proc(i);
		}

#else

		//Parallel foreach
		//Execute the pixel function for every pixel (concurrently)
		QtConcurrent::blockingMap(CountingIterator(0), CountingIterator(height * width), proc);

#endif

		return QPixmap::fromImage(std::move(target));
	}
};
