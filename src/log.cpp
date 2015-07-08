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
	time_t t = std::time(0);
	char buf[sizeof("YYYY-MM-DD HH:MM:SS")];
	return {buf, std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&t))};
}

std::ostream& out(std::ostream* os)
{
	static /*thread_local*/ std::ostream* osp = 0;

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

//std::recursive_mutex mapper::mtx;
//mapper::logmap mapper::logs;
//writer mapper::err(std::cerr);
//
//std::mutex writer::ctx_mtx;
//writer::context_map writer::ctxs;
//
//void writer::set(std::ostream* os)
//{
//	this->ctx = &get_context(*os);
//}
//
//void writer::write_queue()
//{
////	message_que& q = get_q(sink);
//	while(ctx && !ctx->q.empty())
//	{
//		ctx->sink << ctx->q.front().stamp << " [" << id << "] " << ctx->q.front().stream->rdbuf() << this->endl << std::flush;
//		ctx->q.pop();
//	}
//}
//
//void writer::sync()
//{
////	message_que& q = get_q(sink);
//	while(ctx && !done)
//	{
//		std::unique_lock<std::mutex> lock(ctx->mtx);
//		while(!done && ctx && ctx->q.empty())
//			cv.wait(lock);
//		write_queue();
//	}
//}
//
//void writer::start()
//{
//	done = false;
//	fut = std::async(std::launch::async, [this]{sync();});
//}
//
//void writer::stop()
//{
//	done = true;
//
//	cv.notify_one();
//
//	if(fut.valid())
//		fut.get();
//
//	write_queue();
//}
//
//void writer::add_line(sss* ss)
//{
//	if(done || !ctx)
//		return;
//	std::unique_lock<std::mutex> lock(ctx->mtx);
////	message_que& q = get_q(sink);
//	ctx->q.push({get_stamp(), sss_uptr(ss)});
//    cv.notify_one();
//}

}} // sookee::log
