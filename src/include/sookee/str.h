#pragma once
#ifndef _LIBSOOKEE_STR_H_
#define _LIBSOOKEE_STR_H_
/*
 * str.h
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

//#include <sookee/types.h>

#include <functional>
#include <algorithm>

namespace sookee { namespace string {

//using namespace sookee::types;

/**
 * Remove leading characters from a std::string.
 * @param s The std::string to be modified.
 * @param t The set of characters to delete from the beginning
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

/**
 * Remove trailing characters from a std::string.
 * @param s The std::string to be modified.
 * @param t The set of characters to delete from the end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

/**
 * Remove surrounding characters from a std::string.
 * @param s The string to be modified.
 * @param t The set of characters to delete from each end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}

/**
 * Remove all leading characters of a given value
 * from a std::string.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& ltrim(std::string& s, char c)
{
	s.erase(0, s.find_first_not_of(c));
	return s;
}

/**
 * Remove all trailing characters of a given value
 * from a std::string.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& rtrim(std::string& s, char c)
{
	s.erase(s.find_last_not_of(c) + 1);
	return s;
}

/**
 * Remove all surrounding characters of a given value
 * from a std::string.
 * @param s The string to be modified.
 * @param c The character value to delete.
 * @return The same string passed in as a parameter reference.
 */
inline std::string& trim(std::string& s, char c)
{
	return ltrim(rtrim(s, c), c);
}

std::string& replace(std::string& s, const std::string& from, const std::string& to);

inline
std::string& transform(std::string& s, const std::function<int(int)>& func)
{
	std::transform(s.begin(), s.end(), s.begin(), func);
	return s;
}

inline
std::string& lower(std::string& s)
{
	return transform(s, tolower);
}

inline
std::string& lower(std::string&& s)
{
	return lower(s);
}

inline
std::string& upper(std::string& s)
{
	return transform(s, toupper);
}

inline
std::string& upper(std::string&& s)
{
	return upper(s);
}

//inline
//str lowercase(str s)
//{
//	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(tolower));
//	return s;
//}
//
//inline
//str uppercase(str s)
//{
//	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(toupper));
//	return s;
//}

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
 * On failure returns std::string::npos.
 */
size_t extract_delimited_text(const std::string& in, const std::string& d1, const std::string& d2, std::string& out, size_t pos = 0);

/**
 * Split a string into an array of substringd by
 * dividing the string into substrings separated by
 * a given character.
 * @param s The string to divide.
 * @param d The dividing character.
 * @param fold If true, treat any number of the dividing character
 * as a single division. Otherwise each dividing character is one division.
 * @return A std::vector<std::string> containing all the substrings.
 */
std::vector<std::string> split(const std::string& s, char d = ' ', bool fold = true);

}} // sookee::string

namespace soo { using namespace sookee::string; }

#endif /* _LIBSOOKEE_STR_H_ */
