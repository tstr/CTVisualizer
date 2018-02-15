/*
	Image samplers
*/

#pragma once

#include "Volume.h"
#include "VolumeSubimage.h"

/*
	UV coordinates in 2D

	each component is 0.0f-1.0f
*/
struct UV
{
	//image coordinates
	float u;
	float v;
	//image coordinate derivatives
	float ddu;
	float ddv;

	UV(float _u = 0.0f, float _v = 0.0f, float _ddu = 0.0f, float _ddv = 0.0f) :
		u(_u), v(_v), ddu(_ddu), ddv(_ddv)
	{}
};

/*
	UV coordinates in 3D
*/
struct UVW
{
	//image coordinates
	float u;
	float v;
	float w;
	//image coordinate derivatives
	float ddu;
	float ddv;
	float ddw;

	UVW(float _u = 0.0f, float _v = 0.0f, float _w = 0.0f, float _ddu = 0.0f, float _ddv = 0.0f, float _ddw = 0.0f) :
		u(_u), v(_v), w(_w), ddu(_ddu), ddv(_ddv), ddw(_ddw)
	{}
};

/*
	Nearest-Neighbour sampler
*/
class BasicSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, const UV& coords)
	{
		const auto _u = (Volume::Index)(coords.u * (float)image.width());
		const auto _v = (Volume::Index)(coords.v * (float)image.height());

		return image.at(_u, _v);
	}
};

/*
	Bilinear sampler
*/
class BilinearSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, const UV& coords)
	{

	}
};

/*
	Bicubic sampler
*/
class BicubicSampler
{
public:

	static Volume::ElementType sample(const VolumeSubimage& image, const UV& coords)
	{

	}
};

/*
	Trilinear sampler
*/
class TrilinearSampler
{
public:

	static Volume::ElementType sample(const Volume& volume, const UVW& coords)
	{

	}
};
