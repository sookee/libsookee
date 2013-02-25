#pragma once
#ifndef _LIBSOOKEE_LOG_H_
#define _LIBSOOKEE_LOG_H_

/*
 * log.h
 *
 *  Created on: Feb 17, 2013
 *      Author: oasookee@gmail.com
 */

#include <iostream>
#include <ctime>

namespace sookee { namespace log {

/*
 * Basic logging.
 */

inline
std::string get_stamp()
{
	time_t rawtime = std::time(0);
	tm* timeinfo = std::localtime(&rawtime);
	char buffer[32];
	std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", timeinfo);

	return std::string(buffer);
}

inline
std::ostream& out(std::ostream* os = 0)
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

#define log(m) do{sookee::log::out() << oa::get_stamp() << ": " << m << std::endl;}while(false)

// Console output
#define con(m) do{std::cout << m << std::endl;}while(false)

}} // sookee::log

namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
