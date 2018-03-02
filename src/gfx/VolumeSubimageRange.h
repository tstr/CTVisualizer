/*
	Volume Subimage Range helper

	Represents a range of subimages across a given axis in a Volume.

	Allows traversing an axis generically.
*/

#pragma once

#include <iterator>

#include "VolumeSubimage.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

class VolumeSubimageRange
{
public:

	/*
		Subimage iterator
	*/
	class iterator : public std::iterator<std::random_access_iterator_tag, VolumeSubimage, size_t, VolumeSubimage*, VolumeSubimage&>
	{
	private:

		VolumeSubimage m_view;

	public:

		iterator() {}

		iterator(const VolumeSubimage& view) :
			m_view(view)
		{}

		iterator(const iterator& it) :
			m_view(it.m_view)
		{}

		//Arithmetic operations
		iterator& operator++() { m_view.m_index++; return *this; }
		//iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }

		//VolumeSubimage operator-(iterator s) const { VolumeSubimage sub(m_view); sub.m_index - s.m_view.m_index; return sub; }
		//VolumeSubimage operator+(iterator s) const { VolumeSubimage sub(m_view); sub.m_index + s.m_view.m_index; return sub; }
		iterator& operator+=(Volume::IndexType s) { m_view.m_index += s; return *this; }
		iterator& operator-=(Volume::IndexType s) { m_view.m_index -= s; return *this; }

		//Relational operations
		bool operator==(const iterator& rhs) const { return m_view.m_index == rhs.m_view.m_index; }
		bool operator!=(const iterator& rhs) const { return m_view.m_index != rhs.m_view.m_index; }

		//Accessor
		VolumeSubimage& operator*() { return m_view; }
	};

	VolumeSubimageRange(const Volume* volume, VolumeAxis axis) :
		m_begin(volume, 0, axis),
		m_end(volume, volume->axisSize(axis), axis)
	{}
	
	iterator begin() const { return iterator(m_begin); }
	iterator end() const { return iterator(m_end); }

private:

	VolumeSubimage m_begin;
	VolumeSubimage m_end;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
