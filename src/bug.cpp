/*
 * bug.cpp
 *
 *  Created on: 8 Feb 2013
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2013 SooKee oasookee@gmail.com                     |
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
#include <sookee/bug.h>
#include <sookee/str.h>
#include <sookee/ios.h>

//#include <cctype>
//#include <sstream>
//#include <algorithm>
#include <thread>
//#include <numeric>

#include <cstdio>
#include <execinfo.h>
#include <signal.h>
#include <cstdlib>
#include <cxxabi.h>

#include <memory>

namespace sookee { namespace bug {

using namespace sookee::ios;
using namespace sookee::types;
using namespace sookee::utils;

//str thread_name();
//str obj_name(void* id);

#define THREAD sookee::bug::thread_name()
#define OBJECT sookee::bug::obj_name(this)

str thread_name()
{
	std::thread::id id = std::this_thread::get_id();
	static std::map<std::thread::id, siz> thread_map;
	static size_t thread_count(0);

	if(thread_map.find(id) == thread_map.end())
	{
		thread_map[id] = thread_count++;
	}
	return "[" + std::to_string(thread_map[id]) + "]";
}

str obj_name(void* id)
{
	static std::map<void*, siz> obj_map;
	static siz obj_count(0);

	if(obj_map.find(id) == obj_map.end())
	{
		obj_map[id] = obj_count++;
	}
//	return "{" + std::to_string(obj_map[id]) + "}";
	std::ostringstream oss;
	oss << std::hex << id;
	return "{" + std::to_string(obj_map[id]) + ": " + oss.str() + "}";
}

const str COL = "\\[\\033[1;34m\\]";
const str OFF = "\\[\\033[0m\\]";

std::ostream& out(std::ostream* os)
{
	static /*thread_local*/ std::ostream* osp = 0;

	if(!osp) // initialize
		if(!os)
			osp = &std::cout;

	if(os) // change
		osp = os;

	return *osp;
}

str notbug()
{
//	return ansi_esc({NORM});
	return "";
}

str embellish(siz indent)
{
	return std::string(indent, '-');
}

//const int_vec col =
//{
//	FG_RED
//	, FG_GREEN
//	, FG_YELLOW
//	, FG_BLUE
//	, FG_MAGENTA
//	, FG_CYAN
//};

bool do_color = true;

str get_col(const str& /*name*/, int /*seed = 0*/)
{
//	static str_map m;
//
//	if(!bug_do_color)
		return "";

//	if(m.find(name) == m.end())
//		m[name] = ansi_esc({col[std::accumulate(name.begin(), name.end(), seed) % col.size()]});
//
//	return m[name];
}

//#ifdef DEBUG

size_t __scope__bomb__::indent = 0;

// TODO: How can I make this practical???

//static const str_map replacements =
//{
//	{"skivvy::net::basic_socketstream<Char>", "net::socketstream"}
//	, {"skivvy::ircbot::", ""}
//};

//static str fixname(const char* name)
//{
//	str fixed = name;
//
//	if(fixed.find(']') > fixed.find('['))
//	{
//		str line;
//		siss iss(name);
//		sgl(iss, line, '[');
//		fixed = line;
//		sgl(iss, line, ']');
//		sgl(iss, line);
//		fixed += line;
//	}
//
//	for(const std::pair<const str, str>& p: replacements)
//		replace(fixed, p.first, p.second);
//
//	return fixed;
//}

__scope__bomb__::__scope__bomb__(const char* name): name(name)
{
//	bug(name);
	++indent;
	out() << std::endl;
//	bug(get_col(name) << str(indent, '-') + "> " << fixname(name) << ' ' << get_col(THREAD, 1) << THREAD << ' '  << get_col(OBJECT, 2) << OBJECT << ansi_esc({NORM}));
	out() << str(indent, '-') + "> " << name << ' ' << get_col(THREAD, 1) << THREAD << ' '  << get_col(OBJECT, 2) << OBJECT << std::endl;
}
__scope__bomb__::~__scope__bomb__()
{
	out() << "<" << str(indent, '-') << " " << name << ' ' << get_col(THREAD, 1) << THREAD << ' '  << get_col(OBJECT, 2) << OBJECT << std::endl;
//	bug(get_col(name) << "<" << str(indent, '-') << " " << fixname(name) << ' ' << get_col(THREAD, 1) << THREAD << ' '  << get_col(OBJECT, 2) << OBJECT << ansi_esc({NORM}));
	out() << std::endl;
	--indent;
}

void stack_handler(int sig)
{
	void *array[2048];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 2048);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);

	char** trace = backtrace_symbols(array, size);

	int status;
	str obj, func;
	for(siz i = 0; i < size; ++i)
	{
		std::cerr << "raw: " << trace[i] << std::endl;
		siss iss(trace[i]);
		sgl(sgl(iss, obj, '('), func, '+');

		cstring_uptr func_name(abi::__cxa_demangle(func.c_str(), 0, 0, &status));
		std::cerr << "function: " << func_name.get() << '\n';
	}
	free(trace);

	exit(1);
}

}} // sookee::bug
