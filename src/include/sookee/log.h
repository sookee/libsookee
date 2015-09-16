#ifndef _LIBSOOKEE_LOG_H_
#define _LIBSOOKEE_LOG_H_

/*
 * log.h
 *
 *  Created on: Feb 17, 2013
 *      Author: oasookee@gmail.com
 */

#include <cmath>
#include <algorithm>
#include <iostream>

#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <sookee/types/basic.h>
#include <sookee/types/stream.h>

namespace sookee { namespace log {

using namespace sookee::types;

/*
 * Basic logging.
 */

std::string get_stamp();
std::ostream& out(std::ostream* os = 0);

inline std::string get_filename(const std::string& path)
{
	std::string::size_type pos = path.find_last_of("/");
	if(pos != std::string::npos)
		return path.substr(pos + 1);
	return path;
}


#ifndef LIBSOOKEE_MACRO_LOG
#undef LIBSOOKEE_COMPAT_LOG
#define LIBSOOKEE_MACRO_LOG
#endif

// define LIBSOOKEE_MACRO_LOG for old logging
#ifdef LIBSOOKEE_MACRO_LOG

#ifndef DEBUG
#define log(m) do{sookee::log::out() << sookee::log::get_stamp() << ": " << m << std::endl;}while(false)
#else
#define log(m) do{sookee::log::out() << sookee::log::get_stamp() << ": " << m << " [" << sookee::log::get_filename(__FILE__) << "]" << " (" << __LINE__ << ")" << std::endl;}while(false)
#endif

#endif

#ifdef LIBSOOKEE_COMPAT_LOG

#define log_init(os) sookee::log::mapper::create("log", os);
#define log(m) do{sookee::log::mapper::get("log") << m;}while(false)

#endif

#define con(m) do{std::cout << std::boolalpha << m << std::endl;}while(false)
#define con_s(m) do{std::cout << std::boolalpha << m;}while(false)
#define con_e() do{std::cout << std::endl;}while(false)

#define err(m) do{std::cerr << std::boolalpha << m << std::endl;}while(false)

#define wcon(m) do{std::wcout << m << std::endl;}while(false)

//template<typename Return>
//Return& report_error(const str& msg, Return& ret)
//{
//	log(msg);
//}

#define log_return_error(msg, ret) do{log(msg); return ret;}while(0)

//typedef std::unique_ptr<sss> sss_uptr;
//
//class writer
//{
//	friend class mapper;
//
//	struct message
//	{
//		std::string stamp;
//		sss_uptr stream;
//	};
//
//	typedef std::queue<message> message_que;
//	struct context
//	{
//		message_que q;
//		std::mutex mtx;
//		std::ostream& sink;
//		context(std::ostream& sink): sink(sink) {}
//	};
//
//	typedef std::unique_ptr<context> context_uptr;
//	typedef std::map<std::ostream*,context_uptr> context_map;
//	static std::mutex ctx_mtx;
//	static context_map ctxs;
//
//	static context& get_context(std::ostream& os)
//	{
//		lock_guard lock(ctx_mtx);
//		if(!ctxs.count(&os))
//			ctxs.emplace(&os, context_uptr(new context(os)));
//		return *ctxs[&os];
//	}
//
//private:
//	std::future<void> fut;
//	std::condition_variable cv;
////	std::mutex* mtx = nullptr;
////	message_que* q = nullptr;
////	std::ostream* sink = nullptr;
//
//	context* ctx = nullptr;
//
//	std::atomic<bool> done;
//
//	std::string endl = "\n";
//
//	std::string id;
//
//	void sync();
//	void write_queue();
//
//	writer& set_id(const str& id) { this->id = id; return *this; }
//
//public:
//
//	writer(): done(true) { set(nullptr); start(); }
//	writer(std::ostream& sink): done(true)
//	{
//		set(sink);
//		start();
//	}
//	writer(writer&& lw): ctx(lw.ctx), done(lw.done.load())
//	{
//		start();
//	}
//	~writer() { stop(); }
//
//	void set(std::ostream* os);
//	void set(std::ostream& os) { set(&os); }
//	void reset() { set(nullptr); }
//
//	void start();
//
//	void stop();
//
//	void set_endl(char endl) { this->endl = std::string(1, endl); }
//	void set_endl(const std::string& endl) { this->endl = endl; }
//
//	void add_line(sss* ss);
//};
//
//struct buffer
//{
//	writer& lw;
//	sss* ss;
//
//	buffer(writer& lw): lw(lw), ss(new sss) {}
//	buffer(const buffer& buf) = delete;
//	buffer(buffer&& buf): lw(buf.lw), ss(buf.ss) { buf.ss = nullptr; }
//	~buffer() { if(ss) lw.add_line(ss); }
//};
//
//template<typename Type>
//buffer operator<<(buffer&& buf, const Type& t)
//{
//	(*buf.ss) << t;
//	return std::move(buf);
//}
//
//template<typename Type>
//buffer operator<<(writer& lw, const Type& t)
//{
//	return std::move(buffer(lw) << t);
//}
//
//class mapper
//{
//	typedef std::map<str,writer> logmap;
//private:
//	static std::recursive_mutex mtx;
//	static logmap logs;
//
//	static writer err;
//
//public:
//	static writer& create(const str& name, std::ostream& os)
//	{
////		bug_func();
//		recursive_lock_guard lock(mtx);
//		if(!logs.count(name))
//			logs.emplace(name, os);
//		else
//			err << "WARN: calling create on existing log_writer: " << name;
//		return get(name);
//	}
//
//	static writer& get(const str& name)
//	{
////		bug_func();
//		recursive_lock_guard lock(mtx);
//		if(!logs.count(name))
//			err << "WARN: calling get on non-existent log_writer, creating null: " << name;
//		return logs[name].set_id(name);
//	}
//};

// new logging??

class log_writer
{
	std::ostream* sink = nullptr;

	// circular buffer
	static constexpr std::size_t default_size = 30;

	std::vector<std::string> q;
	std::atomic_uint q_head = {0};
	std::atomic_uint q_tail = {0};

	bool enqueue(str msg)
	{
		auto tail = (q_tail + 1) % q.size();

		// full
		if((tail == q_head))
			return false;

		q[q_tail] = std::move(msg);

		q_tail = tail;

		return true;
	}

	bool dequeue(str& msg)
	{
		// empty
		if(q_tail == q_head)
			return false;

		msg = std::move(q[q_head]);

		q_head = (q_head + 1) % q.size();

		return true;
	}

	// sink thread
	std::atomic_bool done = {false};
	std::future<void> fut;

#ifdef DEBUG
	str get_room()
	{
		str room = std::to_string(q_tail - ((q_head + 1) % q.size()) + 1);
		if(room.size() < 3)
			room = str(3 - room.size(), '0') + room;
		return room;
	}

	void sync()
	{
		str msg;
		str room = get_room();
		while(dequeue(msg))
		{
			(*sink) << "[" << room << "] " << msg << '\n';
			room = get_room();
		}
	}
#else
	void sync()
	{
		str msg;
		while(dequeue(msg))
			(*sink) << msg << '\n';
	}
#endif

	void consume()
	{
		while(!done)
		{
			sync();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	struct log_buffer
	{
		log_writer* lw;
		std::stringstream ss;

		void swap(log_buffer& lb)
		{
			if(&lb !=this)
			{
				std::swap(lw, lb.lw);
				std::swap(ss, lb.ss);
			}
		}

		log_buffer(log_writer& lw): lw(&lw) {}
		log_buffer(log_buffer&& lb): lw(lb.lw), ss(std::move(lb.ss)) { lb.lw = nullptr; }
		log_buffer(log_buffer const&) = delete;

		log_buffer& operator=(log_buffer&& lb)
		{
			swap(lb);
			return *this;
		}

		log_buffer& operator=(log_buffer const&) = delete;

		~log_buffer() { if(lw) lw->add_line(ss); }

		template<typename DataType>
		log_buffer operator<<(DataType const& t)
		{
			ss << t;
			return std::move(*this);
		}
	};

	void swap(log_writer& lw)
	{
		if(&lw != this)
		{
//			stop();
//			lw.stop();
			std::swap(sink, lw.sink);
//			lw.start();
//			start();
		}
	}

	std::string stamp()
	{
		std::tm bt;
		time_t t = std::time(0);
		char buf[sizeof("YYYY-MM-DD HH:MM:SS ")];
		return {buf, std::strftime(buf, sizeof(buf), "%F %T ", localtime_r(&t, &bt))};
	}

public:
	log_writer(std::ostream& sink): log_writer(sink, default_size) {}
	log_writer(std::ostream& sink, std::size_t size): sink(&sink), q(size) {}
	log_writer(log_writer&& lw): sink(lw.sink) { lw.sink = nullptr; }
	log_writer(log_writer const&) = delete;

	void reset(std::size_t size = default_size)
	{
		stop();
		q.resize(size);
		q_head = q_tail = 0;
	}

	log_writer& operator=(log_writer&& lw)
	{
		swap(lw);
		return *this;
	}

	log_writer& operator=(log_writer const&) = delete;

	void add_line(std::stringstream& ss)
	{
		str msg = ss.str();
		while(!enqueue(msg))
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	template<typename DataType>
	struct log_buffer operator<<(DataType const& data)
	{
		return std::move(log_buffer(*this) << stamp() << data);
	}

	void start()
	{
		if(done)
		{
			reset(q.size());
			fut = std::async(std::launch::async, &log_writer::consume, this);
		}
	}

	void stop()
	{
		done = true;

		if(fut.valid())
			fut.get();

		sync();
	}
};

}} // sookee::log

namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
