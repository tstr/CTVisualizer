/*
	Effect class for applying effects to a target image
*/
#pragma once

#include <QImage>
#include <QtConcurrentMap>

#include "util/CountingIterator.h"
#include <QDebug>

/*
	Applies a function to every texel in a given target image where the result is stored.
	Similar to a pixel/fragment shader.

	Function must take a UV as an argument and return a byte value
*/
class Effect
{
public:

	template<typename PixelFunc>
	static void apply(QImage& target, const PixelFunc& pixel)
	{
		//Per-pixel procedure
		auto proc = [&](size_t n) {

			const size_t i = n % target.width();
			const size_t j = n / target.width();

			//Relative texture coordinates
			const auto u = (float)i / target.width();
			const auto v = (float)j / target.height();

			//Apply function
			const quint8 col = pixel(UV(u, v));

			//Store result
			target.setPixel((int)i, (int)j, qRgb((int)col, (int)col, (int)col));
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
