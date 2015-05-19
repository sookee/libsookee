#ifndef LIBSOOKEE_REG_H_
#define LIBSOOKEE_REG_H_
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

#include <sookee/types/basic.h>
#include <sookee/types/regex.h>

namespace sookee { namespace reg {

using namespace sookee::types;

// usage:
//
// str s;
// std::regex r;
//
// for(std::smatch& m: smatch_container(s, r))
//     std::cout << m[0] << '\n';
//
class smatch_container
{
	sreg_iter i;

public:
	smatch_container(str_citer b, str_citer e
		, std::regex r, std::regex_constants::match_flag_type m =
        std::regex_constants::match_default)
	: i(b, e, r, m) {}
	smatch_container(
		const str& s
		, std::regex r
		, std::regex_constants::match_flag_type m = std::regex_constants::match_default)
	: i(s.begin(), s.end(), r, m) {}

	sreg_iter begin() const { return i; }
	sreg_iter end() const { return {}; }
};

namespace stock {

const std::regex url(R"~((https?):\/\/([^:\/]*)(?::(\d+))?\/(.*))~");

} // stock
}} // sookee::string

namespace soo { using namespace sookee::reg; }

#endif // LIBSOOKEE_REG_H_
