/*
	Counting iterator helper class
*/

#pragma once

#include <iterator>

class CountingIterator : public std::iterator<std::random_access_iterator_tag, size_t, size_t, size_t, size_t>
{
private:

	size_t count;

public:

	CountingIterator(size_t c = 0) : count(c) {}
	CountingIterator(const CountingIterator& other) : count(other.count) {}

	//Arithmetic operations
	CountingIterator& operator++() { count++; return *this; }
	CountingIterator operator++(int) { CountingIterator tmp(*this); operator++(); return tmp; }

	size_t operator-(CountingIterator s) { return (count - s.count); }
	size_t operator+(CountingIterator s) { return (count + s.count); }
	CountingIterator& operator+=(size_t s) { count += s; return *this; }
	CountingIterator& operator-=(size_t s) { count -= s; return *this; }

	//Relational operations
	bool operator==(const CountingIterator& rhs) const { return count == rhs.count; }
	bool operator!=(const CountingIterator& rhs) const { return count != rhs.count; }

	//Accessor
	size_t operator*() { return count; }
};
