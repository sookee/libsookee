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

#include <sookee/bug.h>
#include <sookee/str.h>
#include <sookee/types.h>

#include <cctype>
#include <sstream>
#include <algorithm>
#include <thread>

namespace sookee { namespace utils {

using namespace sookee::types;

const char* const ws = " \t\n\r\f\v";

str& replace(str& s, const str& from, const str& to)
{
	for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
		s.replace(pos, from.size(), to);
	return s;
}

std::string replace_word(const std::string& s, const std::string& from, const std::string& to)
{
	// comparator functions
	static auto is_space = [](char c){ return std::isspace(c); };
	static auto is_not_space = [](char c){ return !std::isspace(c); };

	std::string r;
	std::string word;

	auto p = s.begin();
	auto e = p;
	const auto end = s.end();

	while(p != end)
	{
		// keep whitespace
		r.append(p, (e = std::find_if(p, end, is_not_space)));

		// extract words (between whitespace)
		word.assign(e, (p = std::find_if(e, end, is_space)));

		// decide to use word or substitute
		if(word == from)
			r.append(to);
		else
			r.append(word);
	}

	return r;
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
	v.reserve(20);
	std::istringstream iss(s);
	str p;
	if(fold)
		while(iss.peek() == d)
			iss.ignore();
	while(std::getline(iss, p, d))
	{
		v.emplace_back(std::move(p));
		if(fold)
			while(iss.peek() == d)
				iss.ignore();
	}
	return v;
}

str_vec split(const str& s, const str& d)
{
	str_vec v;
	v.reserve(20);

	str::size_type pos = 0;
	str::size_type end;

	while((end = s.find(d, pos)) != str::npos)
	{
		v.emplace_back(s.substr(pos, end - pos));
		pos = end + d.size();
	}
	return v;
}

str_vec split2(const std::string& s)
{
	str_vec v;

    auto done = s.end();
    auto end = s.begin();
    decltype(end) pos;

    while((pos = std::find_if(end, done, std::not1(std::ptr_fun(isspace)))) != done)
    {
        end = std::find_if(pos, done, std::ptr_fun(isspace));
        v.emplace_back(pos, end);
    }
    return v;
}

//void split(const str& s, str_vec& v, char d, bool fold)
//{
//	std::istringstream iss(s);
//	str p;
//	while(iss && fold && iss.peek() == d)
//		iss.ignore();
//	while(iss && std::getline(iss, p, d))
//	{
//		v.push_back(p);
//		while(iss && fold && iss.peek() == d)
//			iss.ignore();
//	}
//}


//str_vec split(const str& s, char d, bool fold)
//{
//	str_vec v;
//	std::istringstream iss(s);
//
//	str line;
//	while(std::getline(iss, line, d))
//	{
//		if(fold)
//			trim(line);
//		if(fold && line.empty())
//			continue;
//		v.push_back(line);
//	}
//
//	return v;
//}

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

}} // sookee::utils
