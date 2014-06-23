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

#include <map>
#include <deque>
#include <mutex>

#define TYPEDEF_CONTAINER_T(def) \
typedef def::value_type def##_vt; \
typedef def::iterator def##_iter; \
typedef def::const_iterator def##_citer; \
typedef def::reverse_iterator def##_riter; \
typedef def::const_reverse_iterator def##_criter

/**
 * Single parameter containers
 */
#define TYPEDEF_CONTAINER_1(type, p, def) \
typedef type<p> def; \
TYPEDEF_CONTAINER_T(def)

/**
 * Two parameter containers
 */
#define TYPEDEF_CONTAINER_2(type, p1, p2, def) \
typedef type<p1,p2> def; \
TYPEDEF_CONTAINER_T(def)

#include "types/basic.h"
#include "types/vec.h"
#include "types/map.h"
#include "types/set.h"
#include "types/lst.h"

namespace sookee { namespace types {

typedef std::map<siz, siz> siz_map;

typedef std::map<str, time_t> str_time_map;
typedef std::pair<const str, time_t> str_time_pair;


typedef std::map<str, str> str_map;
typedef std::multimap<str, str> str_mmap;
typedef std::pair<const str, str> str_pair;
typedef std::deque<str> str_deq;

typedef std::map<str, siz> str_siz_map;
typedef std::pair<const str, siz> str_siz_pair;

TYPEDEF_MAP(str, str_set, str_set_map);
//typedef std::map<str, str_set> str_set_map;
//typedef std::pair<const str, str_set> str_set_pair;

typedef std::map<const str, str_vec> str_vec_map;
typedef std::pair<const str, str_vec> str_vec_pair;

typedef std::lock_guard<std::mutex> lock_guard;
typedef std::unique_lock<std::mutex> unique_lock;


}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif /* _LIBSOOKEE_TYPES_H_ */
