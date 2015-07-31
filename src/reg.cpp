/*
 *  Created on: 13 Jul 2015
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee oasookee@gmail.com                     |
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
#include <regex>
#include <string>
#include <sookee/reg.h>

namespace sookee { namespace reg {

using namespace std::regex_constants;

str strerror(error_type e)
{
	static const std::map<error_type, std::string> m
	{
		  { error_collate, "the expression contains an invalid collating element name" }
		, { error_ctype, "the expression contains an invalid character class name" }
		, { error_escape, "the expression contains an invalid escaped character or a trailing escape" }
		, { error_backref, "the expression contains an invalid back reference" }
		, { error_brack, "the expression contains mismatched square brackets ('[' and ']')" }
		, { error_paren, "the expression contains mismatched parentheses ('(' and ')')" }
		, { error_brace, "the expression contains mismatched curly braces ('{' and '}')" }
		, { error_badbrace, "the expression contains an invalid range in a {} expression" }
		, { error_range, "the expression contains an invalid character range (e.g. [b-a])" }
		, { error_space, "there was not enough memory to convert the expression into a finite state machine" }
		, { error_badrepeat, "one of *?+{ was not preceded by a valid regular expression" }
		, { error_complexity, "the complexity of an attempted match exceeded a predefined level" }
		, { error_stack, "there was not enough memory to perform a match" }
	};
	return m.at(e);
}

}} // sookee::reg
