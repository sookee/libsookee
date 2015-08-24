#ifndef LIBSOOKEE_BUG_H
#define LIBSOOKEE_BUG_H
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

#include <iostream>
#include <signal.h>

namespace sookee { namespace bug {

using namespace sookee::types;

//extern bool do_color;
std::ostream& out(std::ostream* os = 0);

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
#define bug_cnt(v)
#define bug_func()
#define ADD_STACK_HANDLER() do{}while(false)
#define bug_lock_guard(lock, mutex) sookee::types::lock_guard lock(mutex)
#else
template<typename Key, typename Val>
std::ostream& operator<<(std::ostream& o, const std::pair<Key, Val>& p)
{
	return o << '{' << p.first << ", " << p.second << '}';
}
#define bug(m) do{sookee::bug::out() << m << std::endl;}while(false)
#define QUOTE(s) #s
#define bug_var(v) bug(QUOTE(v:) << std::boolalpha << " " << v)
#define bug_cnt(v) bug(QUOTE(v:)); for(auto&& e: v) bug("\titem: " << e)
#define bug_func() sookee::bug::__scope__bomb__ __scoper__(__PRETTY_FUNCTION__)
#define ADD_STACK_HANDLER() signal(SIGSEGV, sookee::bug::stack_handler)
struct lock_guard_scope_bomb
{
	const std::string id;
	lock_guard_scope_bomb(const std::string& id): id(id)
	{
		sookee::bug::out() << "LOCK  : " << id << std::endl;
	}
	~lock_guard_scope_bomb()
	{
		sookee::bug::out() << "UNLOCK: " << id << std::endl;
	}
};
#define bug_lock_guard(lock, mutex) sookee::types::lock_guard lock(mutex)
/*
//#define bug_lock_guard(lock, mutex) \
//	sookee::types::soss oss; \
//	oss << __FILE__ << ": " << __func__ << "() " << __LINE__ << " (" << #mutex << ")" << std::endl; \
//	sookee::bug::lock_guard_scope_bomb ____bomb_##lock(oss.str()); \
//	sookee::types::lock_guard lock(mutex)
*/
#endif

#ifndef DEBUG
#define DEBUG_ONLY(code)
#else
#include <sstream>
#define DEBUG_ONLY(code) do{code}while(0)
#define DEBUG_EXCEPTION(message) do{int line = __LINE__; \
std::ostringstream oss; \
oss << message; \
oss << " in " << __FUNCTION__ << " "; \
oss << __FILE__ << " at line " << line; \
throw std::runtime_error(oss.str());}while(0)
#define DEBUG_TEST(expr, message) do{if(expr) DEBUG_EXCEPTION(message);}while(0)
#endif

}} // sookee::bug

namespace soo { using namespace sookee::bug; }

#endif // LIBSOOKEE_BUG_H
