/*
	Pixmap drawing class for filling pixmaps procedurally
*/
#pragma once

#include <QImage>
#include <QtConcurrentMap>

#include "util/CountingIterator.h"
#include "ImageBuffer.h"

class ImageDrawer
{
public:

	/*
		Construct a Pixmap of size w x h where each pixel is computed from a given pixel function.

		Pixel function must take a UV as an argument and return a byte value
	*/
	template<typename PixelFunc>
	static void dispatch(ImageBuffer& target, const PixelFunc& pixel)
	{
		//Per-pixel procedure
		auto proc = [&](size_t n) {

			const quint32 i = (quint32)n % target.width();
			const quint32 j = (quint32)n / target.width();

			//Relative texture coordinates
			const auto u = (float)i / target.width();
			const auto v = (float)j / target.height();

			//Apply function and store result
			target.at(i, j) = pixel(UV(u, v));
		};

#ifdef NO_PARALLEL_PIXEL_FUNC

		//Sequential foreach
		for (size_t i = 0; i < (target.height() * target.width()); i++)
		{
			proc(i);
		}

#else

		//Parallel foreach
		//Execute the pixel function for every pixel (concurrently)
		QtConcurrent::blockingMap(CountingIterator(0), CountingIterator(target.height() * target.width()), proc);

#endif
	}
};
