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

#define log(m) do{sookee::log::out() << sookee::log::get_stamp() << ": " << m << std::endl;}while(false)
#define con(m) do{std::cout << m << std::endl;}while(false)

}} // sookee::log

namespace soo { using namespace sookee::log; }

#endif /* _LIBSOOKEE_LOG_H_ */
