/*
	Image samplers
*/

#pragma once

#include "Volume.h"
#include "VolumeSubimage.h"

/*
	Nearest-Neighbour sampler
*/
class BasicSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, float u, float v)
	{
		const auto _u = (Volume::Coord)floorf(u * (float)image.width());
		const auto _v = (Volume::Coord)floorf(v * (float)image.height());

		return image.at(_u, _v);
	}
};

/*
	Bilinear sampler
*/
class BilinearSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, float u, float v)
	{

	}
};

/*
	Bicubic sampler
*/
class BicubicSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, float u, float v)
	{

	}
};

/*
	Trilinear sampler
*/
class TrilinearSampler
{
public:

	static Volume::ElementType sample(const Volume& volume, float u, float v, float w)
	{

	}
};
