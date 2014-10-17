/*
 *  Created on: Sep 19, 2014
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_MACROS_H_
#define LIBSOOKEE_MACROS_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee <oasookee@gmail.com>				   |
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

#define macro_max(x,y) (((int)((x)<(y)) * (y)) + ((int)((y)<=(x)) * (x)))

#define STRONG_TYPED_STRING(name) \
class name \
: private std::string \
{ \
public: \
	using std::string::string; \
	using std::string::append; \
	using std::string::assign; \
	using std::string::at; \
	using std::string::begin; \
	using std::string::c_str; \
	using std::string::capacity; \
	using std::string::back; \
	using std::string::cbegin; \
	using std::string::cend; \
	using std::string::clear; \
	using std::string::compare; \
	using std::string::data; \
	using std::string::empty; \
	using std::string::end; \
	using std::string::erase; \
	using std::string::find; \
	using std::string::find_first_not_of; \
	using std::string::find_first_of; \
	using std::string::find_last_of; \
	using std::string::find_last_not_of; \
	using std::string::get_allocator; \
	using std::string::insert; \
	using std::string::iterator; \
	using std::string::length; \
	using std::string::max_size; \
	using std::string::npos; \
	using std::string::operator+=; \
	using std::string::operator=; \
	using std::string::operator[]; \
	using std::string::pop_back; \
	using std::string::push_back; \
	using std::string::pointer; \
	using std::string::rbegin; \
	using std::string::rend; \
	using std::string::replace; \
	using std::string::shrink_to_fit; \
	using std::string::size; \
	using std::string::substr; \
	using std::string::swap; \
	using std::string::size_type; \
	using std::string::traits_type; \
	using std::string::value_type; \
	 \
	friend std::istream& operator>>(std::istream& is, mystring& s) \
	{ \
		std::string ss; \
		is >> ss; \
		s = ss; \
		return is; \
	} \
	 \
	friend std::ostream& operator<<(std::ostream& os, const mystring& s) \
	{ \
		return os << static_cast<const std::string&>(s); \
	} \
}

#endif // LIBSOOKEE_MACROS_H_
