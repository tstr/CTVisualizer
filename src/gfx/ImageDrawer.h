/*
	Image drawing class
*/
#pragma once

#include <QtConcurrentMap>

#include "util/CountingIterator.h"
#include "ImageBuffer.h"

class ImageDrawer
{
public:

	/*
		Apply a given pixel function for every pixel in a target image.

		A pixel function in this case is analagous to a pixel/fragment shader:
		The input is the normalized texture coordinates of the destination pixel.
		The output is an 8 bit colour value.
	*/
	template<typename PixelFunc>
	static void dispatch(ImageBuffer& target, const PixelFunc& pixel)
	{
		//Per-pixel procedure
		auto proc = [&](size_t n) {

			//Get coordinates from counting value
			const quint32 i = (quint32)n % target.width();
			const quint32 j = (quint32)n / target.width();

			//Normalized texture coordinates
			const auto u = (float)i / target.width();
			const auto v = (float)j / target.height();

			//Apply function and store result
			target.at(i, j) = pixel(UV(u, v));
		};

		//Optionally parallelism can be disabled when this macro is defined
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
