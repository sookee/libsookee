#ifndef LIBSOOKEE_STR_H_
#define LIBSOOKEE_STR_H_
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

#include <sookee/types/basic.h>
#include <sookee/types/stream.h>
#include <sookee/types/vec.h>

#include <functional>
#include <algorithm>
#include <sstream>
#include <cstring>

namespace sookee { namespace utils {

using namespace sookee::types;

extern const char* const ws;

/**
 * Remove leading characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the beginning
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str& ltrim(str& s, const char* t = ws)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

/**
 * Remove trailing characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str& rtrim(str& s, const char* t = ws)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

/**
 * Remove surrounding characters from a str.
 * @param s The string to be modified.
 * @param t The set of characters to delete from each end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str& trim(str& s, const char* t = ws)
{
	return ltrim(rtrim(s, t), t);
}

// MOVE SEMANTICS

/**
 * Remove leading characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the beginning
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str ltrim(str&& s, const char* t = " \n\t")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

/**
 * Remove trailing characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str rtrim(str&& s, const char* t = " \n\t")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

/**
 * Remove surrounding characters from a str.
 * @param s The string to be modified.
 * @param t The set of characters to delete from each end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str trim(str&& s, const char* t = " \n\t")
{
	return ltrim(rtrim(s, t), t);
}

inline str ltrim_copy(str s, const char* t = ws)
{
	return ltrim(s, t);
}

inline str rtrim_copy(str s, const char* t = ws)
{
	return rtrim(s, t);
}

inline str trim_copy(str s, const char* t = ws)
{
	return trim(s, t);
}

/**
 * Remove all leading characters of a given value
 * from a str.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline str& ltrim(str& s, char c)
{
	s.erase(0, s.find_first_not_of(c));
	return s;
}

/**
 * Remove all trailing characters of a given value
 * from a str.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline str& rtrim(str& s, char c)
{
	s.erase(s.find_last_not_of(c) + 1);
	return s;
}

/**
 * Remove all surrounding characters of a given value
 * from a str.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline str& trim(str& s, char c)
{
	return ltrim(rtrim(s, c), c);
}

inline str rtrim_copy(str s, char c)
{
	return rtrim(s, c);
}

inline str ltrim_copy(str s, char c)
{
	return ltrim(s, c);
}

inline str trim_copy(str s, char c)
{
	return trim(s, c);
}

inline str ltrim_keep(str& s, const char* t = ws)
{
	str::size_type pos;
	str keep = s.substr(0, (pos = s.find_first_not_of(t)));
	s.erase(0, pos);
	return keep;
}

inline str rtrim_keep(str& s, const char* t = ws)
{
	str::size_type pos;
	str keep = s.substr((pos = s.find_last_not_of(t) + 1));
	s.erase(pos);
	return keep;
}

str& replace(str& s, const str& from, const str& to);
// replace whole words only
str replace_word(const str& s, const str& from, const str& to);

inline
str& transform(str& s, const std::function<int(int)>& func)
{
	std::transform(s.begin(), s.end(), s.begin(), func);
	return s;
}

inline
str& lower(str& s)
{
	return transform(s, tolower);
}

inline
str& lower(str&& s)
{
	return lower(s);
}

inline
str& upper(str& s)
{
	return transform(s, toupper);
}

inline
str& upper(str&& s)
{
	return upper(s);
}

inline
str lower_copy(str s)
{
	return lower(s);
}

inline
str upper_copy(str s)
{
	return upper(s);
}

/**
 * Extract a substring from within a string that is delimited by a beginning
 * and an ending string.
 * @param in The string from-which the substring is to be extracted.
 * @param d1 The beginning delimiter. Text AFTER this string will be extracted.
 * @param d2 The end delimiter. Text before this string will be extracted.
 * @param out The returned substring.
 * @param pos The position within the passed in string from which the delimited
 * search is to begin. This allows for repeated calls to this function to extract
 * more than one matching substring.
 * @return On success the position of the character fllowing the end of the end delimiter (d2).
 * On failure returns str::npos.
 */
size_t extract_delimited_text(const str& in, const str& d1, const str& d2, str& out, size_t pos = 0);

/**
 * Split a string into an array of substringd by
 * dividing the string into substrings separated by
 * a given character.
 * @param s The string to divide.
 * @param d The dividing character.
 * @param fold If true, treat any number of the dividing character
 * as a single division. Otherwise each dividing character is one division.
 * @return A std::vector<str> containing all the substrings.
 */
str_vec split(const str& s, char d = ' ', bool fold = true);
//void split(const str& s, str_vec& v, char d = ' ', bool fold = true);

// untested
str_vec split(const str& s, const str& d);

str_vec split2(const std::string& s);

template<typename Container>
str join(const Container& c, const str& delim = " ")
{
	str ret, sep;
	for(const str& s: c)
		{ ret += sep + s; sep = delim; }
	return ret;
}

/**
 * Facilitate easier type conversions
 * <pre>
 * eg.
 * int i;
 * (sss() << "9") >> i;
 * </pre>
 * @param o
 * @param t
 * @return
 */
//template<typename T>
//sss& operator<<(sss&& ss, const T& t)
//{
//	ss << t;
//	return ss;
//}
//
//template<typename T>
//sss& operator>>(sss&& ss, T& t)
//{
//	ss >> t;
//	return ss;
//}

template<typename T>
T to(const str& s)
{
	T t;
	siss iss(s);
	iss >> t;
	return t;
}

// utility function to create char*'s
template<std::size_t Size>
char* make_rptr(const char (&s)[Size])
{
	char* rptr = new char[Size];
	std::strcpy(rptr, s);
	return rptr;
}

}} // sookee::utils

namespace soo { using namespace sookee::utils; }

#endif // LIBSOOKEE_STR_H_
