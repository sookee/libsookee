#pragma once
#ifndef _LIBSOOKEE_STL_H_
#define _LIBSOOKEE_STL_H_
/*
 * stl.h
 *
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

namespace sookee { namespace stl {

template<typename Container, typename T>
size_t count(const Container& c, const T& t)
{
	return std::count(c.begin(), c.end(), t);
}

template<typename Container, typename Pred>
size_t count_if(const Container& c, Pred pred)
{
	return std::count_if(c.begin(), c.end(), pred);
}

// map ready

template<typename Container> inline
typename Container::iterator find(Container& c, const typename Container::key_type& k)
{
	return c.find(k);
}

template<typename Container> inline
typename Container::iterator find(Container& c, const typename Container::value_type& v)
{
	return std::find(c.begin(), c.end(), v);
}

template<typename Container> inline
typename Container::const_iterator find(const Container& c, const typename Container::key_type& k)
{
	return c.find(k);
}

template<typename Container> inline
typename Container::const_iterator find(const Container& c, const typename Container::value_type& v)
{
	return std::find(c.cbegin(), c.cend(), v);
}

// TODO: found not working!!!
template<typename Container> inline
bool found(const Container& c, const typename Container::key_type& k)
{
	return c.find(k) != c.cend(); // find(c, k) != c.cend();
}

// TODO: found not working!!!
template<typename Container> inline
bool found(const Container& c, const typename Container::value_type& v)
{
	return std::find(c.cbegin(), c.cend(), v) != c.cend(); // find(c, v) != c.cend();
}

// end of map ready

template<typename Container, typename Pred> inline
typename Container::iterator find_if(const Container& c, Pred pred)
{
	return std::find_if(c.begin(), c.end(), pred);
}

template<typename Container, typename Pred> inline
typename Container::const_iterator find_if(Container& c, Pred pred)
{
	return std::find_if(c.cbegin(), c.cend(), pred);
}

template<typename Container, typename Comp> inline
void sort(Container& c, Comp& comp)
{
	std::sort(c.begin(), c.end(), comp);
}

template<typename Container> inline
void sort(Container& c)
{
	std::sort(c.begin(), c.end());
}

template<typename Container> inline
void random_shuffle(Container& c)
{
	std::random_shuffle(c.begin(), c.end());
}

template<typename Container, typename Pred>
typename Container::iterator remove_if(Container& c, Pred pred)
{
	return std::remove_if(c.begin(), c.end(), pred);
}

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

}} // sookee::stl

namespace soo { using namespace sookee::stl; }

#endif /* _LIBSOOKEE_STL_H_ */
