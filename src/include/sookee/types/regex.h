#ifndef LIBSOOKEE_TYPES_REGEX_H_
#define LIBSOOKEE_TYPES_REGEX_H_
/*
 *  Created on: 29 Oct 2014
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

#include <regex>

namespace sookee { namespace types {

typedef std::cregex_iterator creg_iter;
typedef std::sregex_iterator sreg_iter;

typedef std::wcregex_iterator wcreg_iter;
typedef std::wsregex_iterator wsreg_iter;

typedef std::cregex_token_iterator creg_titer;
typedef std::sregex_token_iterator sreg_titer;

typedef std::wcregex_token_iterator wcreg_titer;
typedef std::wsregex_token_iterator wsreg_titer;

}} // sookee::types

// namespace soo { using namespace sookee::types; }

#endif // LIBSOOKEE_TYPES_REGEX_H_
