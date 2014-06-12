#ifndef _LIBSOOKEE_TYPES_SET_H_
#define _LIBSOOKEE_TYPES_SET_H_
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

#include <set>

#define TYPEDEF_SET(def, name) \
	TYPEDEF_CONTAINER_1(std::set, def, name)

#define TYPEDEF_MSET(def, name) \
	TYPEDEF_CONTAINER_1(std::multiset, def, name)

#include "str_set.h"
#include "int_set.h"
#include "siz_set.h"
#include "flt_set.h"
#include "dbl_set.h"


#endif // _LIBSOOKEE_TYPES_SET_H_