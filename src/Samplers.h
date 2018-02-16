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

	static Volume::ElementType sample(const VolumeSubimage& view, const UV& coords)
	{
		const auto _u = (Volume::Index)(coords.u * (float)view.width());
		const auto _v = (Volume::Index)(coords.v * (float)view.height());

		return view.at(_u, _v);
	}
};

/*
	Bilinear sampler
*/
class BilinearSampler
{
public:

	//Linear interpolate between two values
	static Volume::ElementType lerp(Volume::ElementType v0, Volume::ElementType v1, float step)
	{
		return (Volume::ElementType)(v0 + (((float)v1 - v0) * step));
	}

	static Volume::ElementType sample(const VolumeSubimage& view, const UV& coords)
	{
		/*
		 (x0,y0)--(x1,y0)	a->
			|	|	  |
			|	g(u,v)|
			|	|	  |
		 (x0,y1)--(x1,y1)   b->

			g = (u,v)
		*/

		//Calculate nearest 4 neighbouring texel coordinates
		float xmin = floorf((coords.u) * (float)view.width());
		float xmax = ceilf((coords.u) * (float)view.width());
		float ymin = floorf((coords.v) * (float)view.height());
		float ymax = ceilf((coords.v) * (float)view.height());

		float x = coords.u * view.width();
		float y = coords.v * view.height();

		//Ensure coordinates are in bounds
		xmin = std::max(xmin, 0.0f);
		xmax = std::min(xmax, (float)view.width() - 1);
		ymin = std::max(ymin, 0.0f);
		ymax = std::min(ymax, (float)view.height() - 1);
		
		const float bias = std::numeric_limits<float>::epsilon();

		//Compute texcoord gradient - bias prevents divide by zero errors
		float xgradient = (x - xmin) / (bias + (xmax - xmin));
		float ygradient = (y - ymin) / (bias + (ymax - ymin));

		//Interpolate on top 2 texels
		const Volume::ElementType a0 = view.at(xmin, ymin);
		const Volume::ElementType a1 = view.at(xmax, ymin);

		const auto alerp = lerp(a0, a1, xgradient);

		//Interpolate on bottom 2 texels
		const Volume::ElementType b0 = view.at(xmin, ymax);
		const Volume::ElementType b1 = view.at(xmax, ymax);

		const auto blerp = lerp(b0, b1, xgradient);

		//Interpolate between two results along the y axis
		return lerp(alerp, blerp, ygradient);
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
