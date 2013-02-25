#pragma once
#ifndef _LIBSOOKEE_BUG_H_
#define _LIBSOOKEE_BUG_H_
/*
 * bug.h
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

#include <ostream>
#include <ctime>
#include <csignal>

namespace sookee { namespace bug {

using namespace sookee::types;

extern bool do_color;
std::ostream& botbug(std::ostream* os = 0);

struct __scope__bomb__
{
	static size_t indent;
	const char* name;
	__scope__bomb__(const char* name);
	~__scope__bomb__();
};

void stack_handler(int sig);

#ifndef DEBUG
#define bug(m)
#define bug_var(v)
#define bug_func()
#define ADD_STACK_HANDLER() do{}while(false)
#else
#define bug(m) do{sookee::bug::botbug() << m << std::endl;}while(false)
#define QUOTE(s) #s
#define bug_var(v) bug(QUOTE(v:) << std::boolalpha << " " << v)
#define bug_func() __scope__bomb__ __scoper__(__PRETTY_FUNCTION__)
#define ADD_STACK_HANDLER() signal(SIGSEGV, sookee::bug::stack_handler)
#endif

}} // sookee::bug

#endif // _LIBSOOKEE_BUG_H_
