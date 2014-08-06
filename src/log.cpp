/*
 * log.cpp
 *
 *  Created on: 25 Feb 2013
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

#include <sookee/log.h>

#include <ctime>

#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>

namespace sookee { namespace log {

std::string get_stamp()
{
	time_t rawtime = std::time(0);
	tm* timeinfo = std::localtime(&rawtime);
	char buffer[32];
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", timeinfo);

	return std::string(buffer);
}

std::ostream& out(std::ostream* os)
{
	static std::ostream* osp = 0;

	// initialize
	if(!osp)
		if(!os)
			osp = &std::cout;

	// change
	if(os)
		osp = os;

	return *osp;
}

// NEW LOGWRITER

void log_writer::sync()
{
	while(!done)
	{
		std::unique_lock<std::mutex> lock(mtx);

		while(q.empty())
			cv.wait(lock);

		while(!q.empty())
		{
			sink << q.front() << this->endl;
			q.pop();
		}
	}
}

void log_writer::start()
{
	done = false;
	fut = std::async(std::launch::async, [this]{sync();});
}

void log_writer::stop()
{
	done = true;
	if(fut.valid())
		fut.get();
	while(!q.empty())
	{
		sink << q.front() << '\n';
		q.pop();
	}
}

void log_writer::add_line(const std::string& msg)
{
	if(done)
		return;

	std::unique_lock<std::mutex> lock(mtx);

	q.push(msg);
	cv.notify_one();
}

}} // sookee::log
