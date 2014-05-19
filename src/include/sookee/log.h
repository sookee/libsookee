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

}} // sookee::log

namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
