#ifndef LIBSOOKEE_TYPES_TYPEDEFS_PAIR_H_
#define LIBSOOKEE_TYPES_TYPEDEFS_PAIR_H_
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

#include <utility>

// deprecated
#define TYPEDEF_PAIR(type1, type2, name) \
	typedef std::pair<type1,type2> name

#define USING_PAIR(type1, type2, name) \
	using name = std::pair<type1,type2>

#endif // LIBSOOKEE_TYPES_TYPEDEFS_PAIR_H_
