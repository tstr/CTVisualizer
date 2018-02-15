/*
	Effect class for applying effects to a target image
*/
#pragma once

#include <QImage>

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
		//Compute coordinate derivatives
		const float ddu = 1.0f / target.width();
		const float ddv = 1.0f / target.height();

		for (int j = 0; j < target.height(); j++)
		{
			for (int i = 0; i < target.width(); i++)
			{
				//Relative texture coordinates
				const auto u = (float)i / target.width();
				const auto v = (float)j / target.height();

				//Apply function
				const quint8 col = pixel(UV( u, v, ddu, ddv ));

				//Store result
				target.setPixel(i, j, qRgb((int)col, (int)col, (int)col));
			}
		}
	}
};
