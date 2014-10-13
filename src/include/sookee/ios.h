#ifndef LIBSOOKEE_IOS_H_
#define LIBSOOKEE_IOS_H_
/*
 *  Created on: 28 Jan 2012
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oasookee@gmail.com                     |
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

#include <sookee/types.h>

#include <istream>
#include <ostream>
#include <limits>

namespace sookee { namespace ios {

using namespace sookee::types;

std::istream& getstring(std::istream& is, str& s);
inline
std::istream& getstring(std::istream&& is, str& s) { return getstring(is, s); }

/**
 * Get directory listing.
 * @return errno
 */
int ls(const str& folder, str_vec &files);

/**
 * Get directory listing.
 * @return errno
 */
bool ls(const str& folder, str_vec& folders, str_vec &files);

enum class ftype
{
	unk, reg, dir, chr, blk, fifo, lnk, sock, all
};

str_vec ls(const str& folder, ftype t = ftype::reg);

inline
bool getargs(std::istream&) { return true; }

template<typename... Args> inline
bool getargs(std::istream& is, str& t, Args&... args);

template<typename T, typename... Args> inline
bool getargs(std::istream& is, T& t, Args&... args)
{
	if(is >> t)
		return getargs(is, args...);
	return false;
}

template<typename... Args> inline
bool getargs(std::istream& is, str& t, Args&... args)
{
	if(ios::getstring(is, t))
		return getargs(is, args...);
	return false;
}

template<typename... Args> inline
bool getargs(const str& s, Args&... args)
{
	std::istringstream iss(s);
	return getargs(iss, args...);
}

/**
 * Like std::ostream_iterator<> but doesn't
 * leave a spare separator at the end.
 */
template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
class ostream_serializer
: public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::basic_ostream<CharT>& os;
	std::basic_string<CharT> sep;
	std::basic_string<CharT> worksep;

public:
	ostream_serializer(std::basic_ostream<CharT>& os
		, const std::basic_string<CharT>& sep = " "): os(os), sep(sep) {}

	ostream_serializer& operator=(const T& value)
    {
		os << worksep << value;
		worksep = sep;
		return *this;
    }

	ostream_serializer& operator*() { return *this; }
	ostream_serializer& operator++() { return *this; }
	ostream_serializer& operator++(int) { return *this; }
};

inline
std::istream& sgl(std::istream& is, str& s, char d = '\n')
{
	return std::getline(is, s, d);
}

inline
std::istream& sgl(std::istream&& is, str& s, char d = '\n')
{
	return sgl(is, s, d);
}

inline
std::istream& ign(std::istream& is, char c = '\n')
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

inline
std::istream& ign(std::istream&& is, char c = '\n')
{
	return ign(is, c);
}

inline
std::istream& user_skip_line(std::istream& is)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename Type>
std::istream& user_get(std::istream& is, Type& t)
{
	if(user_skip_line(is >> t))
		return is;

	// Fix the stream
	is.clear();
	user_skip_line(is).setstate(std::ios::failbit);

	return is;
}

template<typename Type>
std::istream& user_insist(std::istream& is, std::ostream& os, Type& t
	, const str& question, const str& error, siz retries)
{
	os << question;

	while(retries-- && !ios::user_get<int>(is, t))
	{
		is.clear();
		os << error << '\n' << question;
	}

	if(!retries)
		is.setstate(std::ios::failbit);

	return is;
}

// Allow for in >> ',' // parse literal comma
inline std::istream& operator>>(std::istream& is, const char& c)
{
	char i;
	if(is >> i && i != c)
		is.setstate(std::ios::failbit);
	return is;
}

// Allow for in << "  ,  " parse literal string
inline std::istream& operator>>(std::istream& is, const char* s)
{
	char c;
	if(is.setf(std::ios::fmtflags(0)) & std::ios::skipws)
	{
		is >> std::ws;
		while(std::isspace(*s))
			++s;
	}
	while(*s && is.get(c))
		if(*s++ != c)
			is.setstate(std::ios::failbit);
	return is;
}

inline std::istream& operator>>(std::istream& is, const std::string& s)
{
	char c;
	std::string::const_iterator i = s.begin();
	std::string::const_iterator e = s.end();
	if(is.setf(std::ios::fmtflags(0)) & std::ios::skipws)
	{
		is >> std::ws;
		while(std::isspace(*i))
			++i;
	}
	while(i != e && is.get(c))
		if(*i++ != c)
			is.setstate(std::ios::failbit);
	return is;
}

}} // sookee::ios

namespace soo { using namespace sookee::ios; }

#endif // LIBSOOKEE_IOS_H_
