#ifndef LIBSOOKEE_TYPES_TYPEDEFS_MAP_H_
#define LIBSOOKEE_TYPES_TYPEDEFS_MAP_H_
/*
 *  Created on: 21 Jul 2014
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee oasookee@gmail.com                     |
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

#include "typedefs_cnt.h"
#include "typedefs_pair.h"

#include <map>

#define TYPEDEF_MAP(type1, type2, name) \
	TYPEDEF_CONTAINER_2(std::map, type1, type2, name); \
	TYPEDEF_PAIR(name##_iter, name##_iter, name##_range)

#define TYPEDEF_MMAP(type1, type2, name) \
	TYPEDEF_CONTAINER_2(std::multimap, type1, type2, name); \
	TYPEDEF_PAIR(name##_iter, name##_iter, name##_range)

#endif // LIBSOOKEE_TYPES_TYPEDEFS_MAP_H_
