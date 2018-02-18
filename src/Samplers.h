/*
	Image samplers
*/

#pragma once

#include "Volume.h"
#include "VolumeSubimage.h"

/*
	UV coordinates in 2D

	Each component is between 0.0f->1.0f
*/
struct UV
{
	float u;
	float v;

	UV(float _u = 0.0f, float _v = 0.0f) :
		u(_u), v(_v)
	{}
};

/*
	UVW coordinates in 3D
*/
struct UVW
{
	float u;
	float v;
	float w;

	UVW(float _u = 0.0f, float _v = 0.0f, float _w = 0.0f) :
		u(_u), v(_v), w(_w)
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
		const auto _u = (Volume::IndexType)(coords.u * (float)view.width()) % view.width();
		const auto _v = (Volume::IndexType)(coords.v * (float)view.height()) % view.height();

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

		float x = coords.u * view.width();
		float y = coords.v * view.height();

		//Calculate nearest 4 neighbouring texel coordinates
		float xmin = floorf(x);
		float xmax = ceilf(x);
		float ymin = floorf(y);
		float ymax = ceilf(y);

		//Clamp coordinates
		xmin = std::max(xmin, 0.0f);
		xmax = std::min(xmax, (float)view.width() - 1);
		ymin = std::max(ymin, 0.0f);
		ymax = std::min(ymax, (float)view.height() - 1);
		
		const float bias = std::numeric_limits<float>::epsilon();

		//Compute texcoord gradient - bias prevents divide by zero errors
		const float xgradient = (x - xmin) / (bias + (xmax - xmin));
		const float ygradient = (y - ymin) / (bias + (ymax - ymin));

		//Grab 2x2 texel values
		const Volume::ElementType v[4] =
		{
			view.at(xmin, ymin), view.at(xmax, ymin),	//top 2 texels
			view.at(xmin, ymax), view.at(xmax, ymax)	//bottom 2 texels
		};

		//Interpolate between two results along the y axis
		return lerp(
			lerp(v[0], v[1], xgradient),
			lerp(v[2], v[3], xgradient),
			ygradient
		);
	}
};

/*
	Bicubic sampler

	formula derived from this example: http://www.paulinternet.nl/?page=bicubic
*/
class BicubicSampler
{
private:

	//Perform cubic interpolation on 4 control points
	static float interp(float p[4], double i)
	{
		return p[1] + 0.5 * i * (p[2] - p[0] + i*(2.0f*p[0] - 5.0f * p[1] + 4.0f * p[2] - p[3] + i * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
	}

public:

	static Volume::ElementType sample(const VolumeSubimage& view, const UV& coords)
	{
		/*
			16 control points a->p

			a----b----c----d
			|    |    |    |
			|    |    |    |
			e----f----g----h
			|    |    |    |
			|    |    |    |
			i----j----k----l
			|    |    |    |
			|    |    |    |
			m----n----o----p
		*/

		//Convert normalized uv's into coordinates relative to the image we sample
		float x = coords.u * view.width();
		float y = coords.v * view.height();

		float xmin = floorf(x);
		float xmax = ceilf(x);
		float ymin = floorf(y);
		float ymax = ceilf(y);

		const float bias = std::numeric_limits<float>::epsilon();

		//Compute texcoord gradient - bias prevents divide by zero errors
		float xgradient = (x - xmin) / (bias + (xmax - xmin));
		float ygradient = (y - ymin) / (bias + (ymax - ymin));

		//const float xgradient = x - xmin;
		//const float ygradient = y - ymin;

		//Sample within bounds - helper function
		auto sm = [&](Volume::IndexType u, Volume::IndexType v)->float
		{
			//Ensure uv's are in bounds when sampling
			u = std::max(std::min(u, view.width() - 1), 0u);
			v = std::max(std::min(v, view.height() - 1), 0u);
			return (float)view.at(u, v);
		};

		//Fetch values from control points
		float points[4][4] =
		{
			//1. row a->d
			{ sm(xmin - 1, ymin - 1), sm(xmin, ymin - 1), sm(xmax, ymin - 1), sm(xmax + 1, ymin - 1) },
			//2. row e->h
			{ sm(xmin - 1, ymin),     sm(xmin, ymin),     sm(xmax, ymin),     sm(xmax + 1, ymin) },
			//3. row i->l
			{ sm(xmin - 1, ymax),     sm(xmin, ymax),     sm(xmax, ymax),     sm(xmax + 1, ymax) },
			//4. row m->p
			{ sm(xmin - 1, ymax + 1), sm(xmin, ymax + 1), sm(xmax, ymax + 1), sm(xmax + 1, ymax + 1) },
		};

		float results[] = {
			interp(points[0], xgradient),
			interp(points[1], xgradient),
			interp(points[2], xgradient),
			interp(points[3], xgradient)
		};

		return interp(results, ygradient);
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
