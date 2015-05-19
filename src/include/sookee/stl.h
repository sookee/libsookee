#ifndef LIBSOOKEE_STL_H
#define LIBSOOKEE_STL_H
/*
 * SooKee's Template Library
 *
 *  Created on: 1 Aug 2011
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com               |
'------------------------------------------------------------------'

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

http://www.gnu.org/licenses/gpl-2.0.html

'-----------------------------------------------------------------*/

#include <algorithm>
#include <array>

namespace sookee { namespace stl {

// count

template<typename Container>
typename Container::size_type count(const Container& c
	, const typename Container::value_type& t)
{
	return std::count(c.cbegin(), c.cend(), t);
}

template<typename Container, typename Pred>
typename Container::size_type count_if(const Container& c, Pred pred)
{
	return std::count_if(c.cbegin(), c.cend(), pred);
}

template<typename Container>
typename Container::size_type count_key(const Container& c
	, const typename Container::key_type& t)
{
	return c.count(t);
}

// find

template<typename Container>
typename Container::iterator find(Container& c
	, const typename Container::value_type& v)
{
	return std::find(c.begin(), c.end(), v);
}

template<typename Container, typename Pred>
typename Container::iterator find_if(Container& c, Pred pred)
{
	return std::find_if(c.begin(), c.end(), pred);
}

template<typename Container>
typename Container::iterator find_key(Container& c
	, const typename Container::key_type& k)
{
	return c.find(k);
}

//// const find

template<typename Container>
typename Container::const_iterator find(const Container& c
	, const typename Container::value_type& v)
{
	return std::find(c.cbegin(), c.cend(), v);
}

template<typename Container, typename Pred>
typename Container::const_iterator find_if(const Container& c, Pred pred)
{
	return std::find_if(c.cbegin(), c.cend(), pred);
}

template<typename Container>
typename Container::const_iterator find_key(const Container& c
	, const typename Container::key_type& k)
{
	return c.find(k);
}

// mutate

template<typename Container, typename Comp>
void sort(Container& c, Comp comp)
{
	std::sort(c.begin(), c.end(), comp);
}

template<typename Container>
void sort(Container& c)
{
	std::sort(c.begin(), c.end());
}

template<typename Container>
void random_shuffle(Container& c)
{
	std::random_shuffle(c.begin(), c.end());
}

template<typename Container, typename Pred>
typename Container::iterator remove(Container& c
	, const typename Container::value_type& t)
{
	return std::remove(c.begin(), c.end(), t);
}

template<typename Container, typename Pred>
typename Container::iterator remove_if(Container& c, Pred pred)
{
	return std::remove_if(c.begin(), c.end(), pred);
}

// untested

template<typename Container, typename Pred>
typename Container::iterator erase_if(Container& c, Pred pred)
{
	return c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

template<typename Container> inline
void copy(const Container& c1, Container& c2)
{
	std::copy(c1.begin(), c1.end(), c2.begin());
}

template<typename Iter>
class IterRange
{
	Iter s;
	Iter e;

public:
	IterRange(const Iter& start, const Iter &end): s(start), e(end) {}

	Iter begin() { return s; }
	Iter end() { return e; }
};

template<typename T>
IterRange<T> make_range(const T &start, const T &end)
{
	return IterRange<T>(start, end);
}

namespace experimental {

template<typename Type, unsigned SIZE>
class circular
{
	size_t pos = 0;
	std::array<Type, SIZE> data;

	void add(const Type& v)
	{
		data[pos++] = v;
		if(pos == SIZE)
			pos = 0;
	}

	const Type& operator[](size_t idx) const { idx += pos; if(idx >= SIZE) idx -= SIZE; return data[idx]; }

	struct iterator
	{
		size_t pos = 0;
		circular* c;
		iterator(): c(nullptr) {}
		iterator(circular* c): pos(c->pos), c(c) {}

	    iterator(const iterator& i): pos(i.pos), c(i.c) {}

	    iterator& operator=(const iterator& i) { pos = i.pos; c = i.c; return *this; }
	    iterator& operator++() { ++pos; if(pos == SIZE) pos = 0; if(pos == c->pos) c = nullptr; return *this; } //prefix increment
	    Type& operator*() const { return c->data[pos]; }
	};
};

// EXAMPLE ITERATOR
template<typename Type>
struct base_iterator
{
	using value_type = Type;
	using pointer = value_type*;
	using const_pointer = value_type const*;
	using reference = value_type&;
	using const_reference = value_type const&;

	pointer ptr = nullptr;

//	virtual ~basic_iterator() {}
	base_iterator(pointer ptr): ptr(ptr) {}
	base_iterator(base_iterator&& iter): ptr(iter.ptr) { iter.ptr = nullptr; }
	base_iterator(const base_iterator& iter): ptr(iter.ptr) {}

	bool operator==(base_iterator& iter) const { return ptr == iter.ptr; }
	bool operator!=(base_iterator& iter) const { return !(*this == iter); }

	reference operator*() { return *ptr; }
	const_reference operator*() const { return *ptr; }

	pointer operator->() { return ptr; }
	const_pointer operator->() const { return ptr; }

	base_iterator& operator++() { ++ptr; return *this; }
	base_iterator operator++(int) { base_iterator iter(*this); ++ptr; return iter; }
	base_iterator& operator--() { --ptr; return *this; }
	base_iterator operator--(int) { base_iterator iter(*this); --ptr; return iter; }
};

} // experimental

}} // sookee::stl

namespace soo { using namespace sookee::stl; }

#endif // LIBSOOKEE_STL_H
