#pragma once
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

}} // sookee::log

// namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
