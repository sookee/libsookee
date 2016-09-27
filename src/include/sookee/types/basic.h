#ifndef LIBSOOKEE_TYPES_BASIC_H
#define LIBSOOKEE_TYPES_BASIC_H
/*
 *  Created on: 9 Jan 2012
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

#include <string>
#include <chrono>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>

namespace sookee { namespace types {

typedef unsigned int uns;
typedef unsigned char byte;
typedef std::string str;
typedef str::size_type siz;
typedef str::iterator str_iter;
typedef str::const_iterator str_citer;

using str_pair = std::pair<str, str>;

// time
//typedef std::chrono::steady_clock st_clk;
typedef std::chrono::system_clock st_clk;
typedef st_clk::period st_period;
typedef st_clk::duration st_duration;
typedef st_clk::time_point st_time_point;

typedef std::chrono::high_resolution_clock hr_clk;
typedef hr_clk::period hr_period;
typedef hr_clk::duration hr_duration;
typedef hr_clk::time_point hr_time_point;

typedef std::lock_guard<std::mutex> lock_guard;
typedef std::unique_lock<std::mutex> unique_lock;
typedef std::lock_guard<std::recursive_mutex> recursive_lock_guard;

template<typename Type>
using sptr = std::shared_ptr<Type>;

template<typename Type>
using uptr = std::unique_ptr<Type>;

template<typename Type>
using rptr = Type*;

struct malloc_deleter
{
	template <class T>
	void operator()(T* p) { std::free(p); }
};

typedef std::unique_ptr<char, malloc_deleter> cstring_uptr;

// type deduction helper
template<typename Type>
class compiletime_type_is;

// Static type info

template <typename T>
struct TypeId {
};

template <size_t N>
struct TypeInfo {
};

#define MAKE_TYPE_ID(stype, id)\
template <> \
struct TypeId<stype> { \
	static const size_t value = id; \
}; \
template <> \
struct TypeInfo<TypeId<stype>::value> { \
    typedef stype type; \
    static const size_t size = sizeof(type); \
    static constexpr char const* name = #stype; \
}

MAKE_TYPE_ID(int, 1);
MAKE_TYPE_ID(unsigned int, 2);
MAKE_TYPE_ID(long int, 3);
MAKE_TYPE_ID(unsigned long int, 4);
MAKE_TYPE_ID(long long int, 5);
MAKE_TYPE_ID(unsigned long long int, 6);
MAKE_TYPE_ID(float, 7);
MAKE_TYPE_ID(double, 8);

}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif // LIBSOOKEE_TYPES_BASIC_H
