#pragma once
#ifndef _LIBSOOKEE_TYPES_H_
#define _LIBSOOKEE_TYPES_H_
/*
 * tyoes.h
 *
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

#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <memory>
#include <cstdlib>

#include <map>
#include <set>
#include <list>
#include <deque>
#include <mutex>
#include <stack>
#include <vector>
#include <random>
#include <chrono>
#include <istream>
#include <utility>

namespace sookee { namespace types {

typedef std::size_t siz;

typedef std::string str;
typedef str::iterator str_iter;
typedef str::const_iterator str_citer;

typedef std::vector<int> int_vec;
typedef std::vector<siz> siz_vec;
typedef std::map<siz, siz> siz_map;

typedef std::vector<str> str_vec;
typedef str_vec::iterator str_vec_itr;
typedef str_vec::const_iterator str_vec_citr;

typedef std::list<str> str_list;
typedef str_list::iterator str_list_iter;
typedef str_list::const_iterator str_list_citer;

typedef std::set<str> str_set;
typedef str_set::const_iterator str_set_citer;

typedef std::map<str, time_t> str_time_map;
typedef std::pair<const str, time_t> str_time_pair;


typedef std::multiset<str> str_mset;
typedef std::map<str, str> str_map;
typedef std::multimap<str, str> str_mmap;
typedef std::pair<const str, str> str_pair;
typedef std::deque<str> str_deq;

typedef std::map<str, siz> str_siz_map;
typedef std::pair<const str, siz> str_siz_pair;

typedef std::map<str, str_set> str_set_map;
typedef std::pair<const str, str_set> str_set_pair;

typedef std::map<const str, str_vec> str_vec_map;
typedef std::pair<const str, str_vec> str_vec_pair;

typedef std::lock_guard<std::mutex> lock_guard;
typedef std::unique_lock<std::mutex> unique_lock;

// time
typedef std::chrono::steady_clock st_clk;
typedef st_clk::period st_period;
typedef st_clk::time_point st_time_point;

typedef std::chrono::high_resolution_clock hr_clk;
typedef hr_clk::period hr_period;
typedef hr_clk::time_point hr_time_point;

typedef std::stringstream sss;
typedef std::istringstream siss;
typedef std::ostringstream soss;

typedef std::fstream sfs;
typedef std::ifstream sifs;
typedef std::ofstream sofs;

typedef std::stringstream sss;

inline
std::istream& sgl(std::istream& is, str& s, char d = '\n')
{
	return std::getline(is, s, d);
}

inline
std::istream& sgl(std::istream&& is, str& s, char d = '\n')
{
	return sgl(is, s, d);
}

struct malloc_deleter
{
	template <class T>
	void operator()(T* p) { std::free(p); }
};

typedef std::unique_ptr<char, malloc_deleter> cstring_uptr;

}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif /* _LIBSOOKEE_TYPES_H_ */
