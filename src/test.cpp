/*
 * test.cpp
 *
 *  Created on: 23 Apr 2014
 *      Author: oasookee@gmail.com
 */

#include <sookee/types.h>
//#include <sookee/str.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/radp.h>

const char* const ws = " \t\n\r\f\v";

inline str& ltrim_mute(str& s, const char* t = ws)
{
	bug_func();
	s.erase(0, s.find_first_not_of(t));
	return s;
}

inline str ltrim_move(str&& s, const char* t = ws)
{
	bug_func();
	return std::move(ltrim_mute(s, t));
}

inline str ltrim_cref(const str& s, const char* t = ws)
{
	bug_func();
	return std::move(ltrim_move(str(s),t));
}

inline str ltrim_copy(str s, const char* t = ws)
{
	bug_func();
	return std::move(ltrim_move(std::move(s),t));
}

/**
 * Remove leading characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the beginning
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
inline str& ltrim(str& s, const char* t = ws)
{
	bug_func();
	return ltrim_mute(s, t);
}

inline str ltrim(str&& s, const char* t = ws)
{
	bug_func();
	return ltrim_move(std::move(s), t);
}

inline str ltrim(const str& s, const char* t = ws)
{
	bug_func();
	return ltrim_cref(s, t);
}

/**
 * Remove trailing characters from a str.
 * @param s The str to be modified.
 * @param t The set of characters to delete from the end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
//inline str& rtrim(str& s, const char* t = ws)
//{
//	bug_func();
//	s.erase(s.find_last_not_of(t) + 1);
//	return s;
//}

/**
 * Remove surrounding characters from a str.
 * @param s The string to be modified.
 * @param t The set of characters to delete from each end
 * of the string.
 * @return The same string passed in as a parameter reference.
 */
//inline str& trim(str& s, const char* t = ws)
//{
//	bug_func();
//	return ltrim(rtrim(s, t), t);
//}

//inline str ltrim_copy(str s, const char* t = ws)
//{
//	bug_func();
//	return ltrim(s, t);
//}

//inline str rtrim_copy(str s, const char* t = ws)
//{
//	bug_func();
//	return rtrim(s, t);
//}

//inline str trim_copy(str s, const char* t = ws)
//{
//	bug_func();
//	return trim(s, t);
//}

int main()
{
	str a = " a ";
	const str c = " c ";

	ltrim(a);
	ltrim(" b ");
	ltrim(c);



}
