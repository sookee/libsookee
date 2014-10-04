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
#include <sookee/test.h>

#include <boost/algorithm/string/trim.hpp>

using namespace sookee::types;
using namespace sookee::test;

// TODO:
// std::string& trim_mute(std::string&& s...) ???
// timings for rtrim() methods

namespace soo2 {

namespace ws {
const char* const all = " \t\n\r\f\v";
const char* const space = " ";
const char* const tabs = "\t";
const char* const indentation = " \t";

//template<typename _CharT>
//bool isalpha(_CharT, const std::locale&);

} // ::ws

// PRIMATIVES

inline std::string& ltrim_mute(std::string& s, const char* chars = ws::indentation)
{
	s.erase(0, s.find_first_not_of(chars));
	return s;
}

// TODO: Is this legit?
inline std::string& ltrim_mute1(std::string&& s, const char* chars = ws::indentation)
{
	s.erase(0, s.find_first_not_of(chars));
	return s;
}

// TODO: Is this legit?
inline std::string& ltrim_mute2(std::string&& s, const char* chars = ws::indentation)
{
	return ltrim_mute(s, chars);
}

inline std::string ltrim(const std::string& s, const char* chars = ws::indentation)
{
	size_t pos = s.find_first_not_of(chars);
	if(pos == std::string::npos)
		return s;
	return s.substr(pos);
}

inline std::string ltrim(std::string&& s, const char* chars = ws::indentation)
{
	return ltrim(s, chars);
}

// RIGHT

inline std::string& rtrim_mute(std::string& s, const char* chars = ws::indentation)
{
	s.erase(s.find_last_not_of(chars) + 1);
	return s;
}

inline std::string rtrim(const std::string& s, const char* chars = ws::indentation)
{
	size_t pos = s.find_last_not_of(chars);
	if(pos == std::string::npos)
		return s;
	return s.substr(0, pos + 1);
}

inline std::string rtrim(std::string&& s, const char* chars = ws::indentation)
{
	return rtrim(s, chars);
}

// BOTH

inline std::string& trim_mute(std::string& s, const char* chars = ws::indentation)
{
	return ltrim_mute(rtrim_mute(s, chars), chars);
}

inline std::string trim(const std::string& s, const char* chars = ws::indentation)
{
	return ltrim(rtrim(s, chars), chars);
}

inline std::string trim(std::string&& s, const char* chars = ws::indentation)
{
	return ltrim(rtrim(s, chars), chars);
}

//template<typename String, typename Char>
//String& ltrim_mute(String& s, Char const* chars)
//{
//	s.erase(0, s.find_first_not_of(chars));
//}

typedef char Char;
typedef std::vector<Char> String;

//String::iterator find_first_not_of(String const& s, Char const* chars)
//{
//	String::iterator found = s.begin();
//	for(found = s.begin(); found != s.end(); ++found)
//		for(Char const* p = chars; *p; ++p)
//			if(*p == *found)
//
//	return found;
//}
//
//String& ltrim_mute(String& s, Char const* chars)
//{
//	s.erase(0, s.find_first_not_of(chars));
//
//	return s;
//}

} // ::soo

std::string convert(const std::string& s)
{
	std::string out;
	for(char c: s)
	{
		if(c == '\t')
			out += "\\t";
		else if(c == '\n')
			out += "\\n";
		else if(c == '\r')
			out += "\\r";
		else if(c == '\f')
			out += "\\f";
		else if(c == '\v')
			out += "\\v";
		else
			out += c;
	}
	return out;
}

const std::string test_v[] =
{
	""
	, "test"
	, " test"
	, " \ttest"
	, " \t \ttest"
	, " \t \t \ttest"
};

#include <sookee/str.h>

str& replace1(str& s, const str& from, const str& to)
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

str& replace2(str& s, const str& from, const str& to)
{
	for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
		s.replace(pos, from.size(), to);
	return s;
}

str replace3(str s, const str& from, const str& to)
{
	for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
		s.replace(pos, from.size(), to);
	return s;
}

void time_replace()
{
	timespec tsb;
	timespec tse;
	siz iterations = 10000000;

	const str test_s = "some text to replace";
	const str from = "t";
	const str to = "wibble";
	str s;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < iterations; ++i)
	{
		s = test_s;
		s = replace1(s, from, to);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	con(s);
	con("r1: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < iterations; ++i)
	{
		s = test_s;
		s = replace2(s, from, to);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	con(s);
	con("r2: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < iterations; ++i)
	{
		s = test_s;
		s = replace3(s, from, to);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	con(s);
	con("r3: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));
}

int main()
{

	time_replace();
	return 1;

	timespec tsb;
	timespec tse;
	siz iterations = 100000000;

	//auto ws = soo2::ws::indentation;

	for(const auto& ws: {soo2::ws::indentation})
	{
		for(const auto& test_s: test_v)
		{
			con("|--------------------------------------");
			con("| test: " << convert(test_s));
			con("|   ws: " << convert(ws));
			con("|--------------------------------------");
			con("|  NEW: ");
			con("|--------------------------------------");

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
			for(siz i = 0; i < iterations; ++i)
			{
				str s = test_s;
				soo2::ltrim_mute(s, ws);
			}
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

			con("mute0: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
			for(siz i = 0; i < iterations; ++i)
			{
				str s = test_s;
				soo2::ltrim_mute1(std::move(s));
			}
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

			con("mute1: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
			for(siz i = 0; i < iterations; ++i)
			{
				str s = test_s;
				soo2::ltrim_mute2(std::move(s));
			}
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

			con("mute2: " << (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec));
		}
	}
}












