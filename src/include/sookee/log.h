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

#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <sstream>

namespace sookee { namespace log {

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

#ifndef DEBUG
#define log(m) do{sookee::log::out() << sookee::log::get_stamp() << ": " << m << std::endl;}while(false)
#else
#define log(m) do{sookee::log::out() << sookee::log::get_stamp() << ": " << m << " [" << sookee::log::get_filename(__FILE__) << "]" << " (" << __LINE__ << ")" << std::endl;}while(false)
#endif
#define con(m) do{std::cout << m << std::endl;}while(false)
#define con_s(m) do{std::cout << m;}while(false)
#define con_e() do{std::cout << std::endl;}while(false)

class log_writer
{
private:
	std::mutex mtx;
	std::future<void> fut;
	std::condition_variable cv;
	std::queue<std::string> q;

	std::ostream& sink;
	std::atomic<bool> done;

	std::string endl = "\n";

	void sync();

public:

	log_writer(std::ostream& sink): sink(sink), done(false) {}
	~log_writer() { stop(); }

	void start();

	void stop();

	void set_endl(char endl) { this->endl = std::string(1, endl); }
	void set_endl(const std::string& endl) { this->endl = endl; }

	void add_line(const std::string& msg);

};

struct log_buffer
{
	log_writer& lw;
	std::ostringstream* oss;

	log_buffer(log_writer& lw): lw(lw), oss(new std::ostringstream) {}
	log_buffer(const log_buffer& buf) = delete;
	log_buffer(log_buffer&& buf): lw(buf.lw), oss(buf.oss) { buf.oss = 0; }
	~log_buffer() { if(oss) lw.add_line(oss->str()); delete oss; }
};

template<typename Type>
log_buffer operator<<(log_buffer&& buf, const Type& t)
{
	(*buf.oss) << t;
	return std::move(buf);
}

template<typename Type>
log_buffer operator<<(log_writer& lw, const Type& t)
{
	return std::move(log_buffer(lw) << t);
}

}} // sookee::log

namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
