/*
 *  Created on: Mar 5, 2015
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_SCOPER_H_
#define LIBSOOKEE_SCOPER_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee <oasookee@gmail.com>				   |
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

namespace sookee { namespace utils {

template<typename Type, typename Functor>
struct scoper
{
	Type& obj;
	Functor func;
	scoper(Type& obj): obj(obj) {}
	~scoper() { func(obj); }
};

template<typename Type>
struct clear_functor
{
	void operator()(Type& obj) { obj.clear(); }
};

template<typename Type>
using clear_scoper = scoper<Type, clear_functor<Type>>;

template<typename Type>
struct free_functor
{
	void operator()(Type& obj) { free(obj); }
};

template<typename Type>
using free_scoper = scoper<Type, free_functor<Type>>;

template<typename Type>
auto make_clear_scoper(Type& obj) { return clear_scoper<Type>(obj); }

template<typename Type>
auto make_free_scoper(Type& obj) { return free_scoper<Type>(obj); }

}} // ::sookee::utils

namespace soo { using namespace sookee::utils; }

#endif // LIBSOOKEE_SCOPER_H_
