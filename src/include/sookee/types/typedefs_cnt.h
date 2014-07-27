#ifndef LIBSOOKEE_TYPES_TYPEDEFS_CNT_H_
#define LIBSOOKEE_TYPES_TYPEDEFS_CNT_H_
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

#endif // LIBSOOKEE_TYPES_TYPEDEFS_CNT_H_