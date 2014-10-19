#pragma once
#ifndef LIBSOOKEE_TYPES_STREAM_H_
#define LIBSOOKEE_TYPES_STREAM_H_
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

#include <fstream>
#include <sstream>
#include <iterator>

namespace sookee { namespace types {

typedef std::istream sis;
typedef std::ostream sos;
typedef std::iostream sios;

typedef std::stringstream sss;
typedef std::istringstream siss;
typedef std::ostringstream soss;

typedef std::fstream sfs;
typedef std::ifstream sifs;
typedef std::ofstream sofs;

typedef std::istream_iterator<char> sisi;
typedef std::ostream_iterator<char> sosi;
typedef std::istreambuf_iterator<char> sisbi;
typedef std::ostreambuf_iterator<char> sosbi;

}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif // LIBSOOKEE_TYPES_STREAM_H_