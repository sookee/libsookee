/*
 * str.cpp
 *
 *  Created on: 28 Jan 2012
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oasookee@gmail.com               |
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

#include <sookee/str.h>
#include <sookee/types.h>

#include <cctype>
#include <sstream>
#include <algorithm>
#include <thread>

namespace sookee { namespace string {

using namespace sookee::types;

str& replace(str& s, const str& from, const str& to)
{
	if(from.empty())
		return s;
	siz pos = 0;
	if((pos = s.find(from)) != str::npos)
		s.replace(pos, from.size(), to);
	while((pos = s.find(from, pos + to.size())) != str::npos)
		s.replace(pos, from.size(), to);
	return s;
}

str::size_type extract_delimited_text(const str& in, const str& d1, const str& d2, str& out, size_t pos)
{
	if(pos == str::npos)
		return pos;

	size_t end = pos;

	if((pos = in.find(d1, pos)) != str::npos)
		if((end = in.find(d2, (pos = pos + d1.size()))) != str::npos)
		{
			out = in.substr(pos, end - pos);
			return end + d2.size();
		}
	return str::npos;
}

str_vec split(const str& s, char d, bool fold)
{
	str_vec v;
	std::istringstream iss(s);
	str p;
	while(fold && iss.peek() == d)
		iss.ignore();
	while(std::getline(iss, p, d))
	{
		v.push_back(p);
		while(fold && iss.peek() == d)
			iss.ignore();
	}
	return v;
}

//// TODO: Move this to <sookee/stl.h>
//template<typename Container, typename Pred>
//typename Container::iterator remove_if(Container& c, Pred pred)
//{
//	return std::remove_if(c.begin(), c.end(), pred);
//}
//
//// TODO: Move this to <sookee/stl.h>
//template<typename Container, typename Pred>
//typename Container::iterator erase_if(Container& c, Pred pred)
//{
//	return c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
//}

}} // sookee::string
