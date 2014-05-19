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

#include <sookee/bug.h>
#include <sookee/str.h>
//#include <sookee/ansi.h>

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

namespace sookee { namespace log {

//using namespace sookee::ansi;
using namespace sookee::types;
using namespace sookee::string;

std::ostream& out(std::ostream* os)
{
	static std::ostream* osp = 0;

	if(!osp) // initialize
		if(!os)
			osp = &std::cout;

	if(os) // change
		osp = os;

	return *osp;
}

}} // sookee::bug
