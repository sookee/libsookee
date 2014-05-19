#pragma once
#ifndef _LIBSOOKEE_LOG_H_
#define _LIBSOOKEE_LOG_H_
/*
 * log.h
 *
 *  Created on: 19 May 2014
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

namespace sookee { namespace log {

using namespace sookee::types;

std::ostream& out(std::ostream* os = 0);

inline
str get_stamp()
{
	time_t rawtime = std::time(0);
	tm* timeinfo = std::localtime(&rawtime);
	char buffer[32];
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", timeinfo);

	return str(buffer);
}

#ifndef DEBUG
#define con(m) do{std::cout << m << std::endl;}while(false)
#define log(m) do{std::cout << sookee::log::get_stamp() << ": " << m << std::endl;}while(false)
#else
#define con(m) do{std::cout << m << " [" << __FILE__ << "]" << " (" << __LINE__ << ")" << std::endl;}while(false)
#define log(m) do{std::cout << sookee::log::get_stamp() << ": " << m << " [" << __FILE__ << "]" << " (" << __LINE__ << ")" << std::endl;}while(false)
#endif

}} // sookee::log

#endif // _LIBSOOKEE_LOG_H_
