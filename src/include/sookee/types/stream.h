#ifndef LIBSOOKEE_TYPES_STREAM_H_
#define LIBSOOKEE_TYPES_STREAM_H_
/*
 *  Created on: 9 Jan 2012
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com                     |
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

using sis = std::istream;
using sos = std::ostream;
using sios = std::iostream;

using sss = std::stringstream;
using siss = std::istringstream;
using soss = std::ostringstream;

using sfs = std::fstream;
using sifs = std::ifstream;
using sofs = std::ofstream;

using sis_iter = std::istream_iterator<char>;
using sos_iter = std::ostream_iterator<char>;
using sisb_iter = std::istreambuf_iterator<char>;
using sosb_iter = std::ostreambuf_iterator<char>;

}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif // LIBSOOKEE_TYPES_STREAM_H_
