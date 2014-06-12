#ifndef _LIBSOOKEE_TYPES_MAP_H_
#define _LIBSOOKEE_TYPES_MAP_H_
/*
 *  Created on: 10 June 2014
 *      Author: SooKee oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee oasookee@gmail.com               |
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

#include "basic.h"
#include "pair.h"

#include <map>

#define TYPEDEF_MAP(type1, type2, name) \
	TYPEDEF_CONTAINER_2(std::map, type1, type2, name); \
	TYPEDEF_PAIR(name##_iter, name##_iter, name##_range)

#define TYPEDEF_MMAP(type1, type2, name) \
	TYPEDEF_CONTAINER_2(std::multimap, type1, type2, name); \
	TYPEDEF_PAIR(name##_iter, name##_iter, name##_range)

#include "str_map.h"
#include "int_map.h"
#include "siz_map.h"
#include "flt_map.h"
#include "dbl_map.h"

#endif // _LIBSOOKEE_TYPES_MAP_H_
