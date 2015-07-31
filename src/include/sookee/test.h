#ifndef LIBSOOKEE_TEST_H_
#define LIBSOOKEE_TEST_H_

/*
 *  Created on: Aug 6, 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <functional>
#include <iostream>

#include <sookee/types/basic.h>

namespace sookee { namespace test {

using namespace sookee::types;

struct context
{
	str input;
	str expect;
};

class PosixTimer
{
	timespec tsb;
	timespec tse;

public:
	void clear() { start(); tse = tsb; }
	void start() { clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb); }
	void stop() { clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse); }

	friend std::ostream& operator<<(std::ostream& o, const PosixTimer& timer)
	{
		return o << (double) (timer.tse.tv_nsec - timer.tsb.tv_nsec) / 1000000000 + (double) (timer.tse.tv_sec - timer.tsb.tv_sec);
	}
};

class Timer
{
	hr_clk::time_point tsb;
	hr_clk::time_point tse;

public:

	void clear() { tsb = tse = hr_clk::now(); }
	void start() { tsb = hr_clk::now(); }
	void stop() { tse = hr_clk::now(); }

	friend std::ostream& operator<<(std::ostream& o, const Timer& timer)
	{
		return o << timer.secs();
	}

	// return time difference in seconds
	double secs() const
	{
		if(tse <= tsb)
			return 0.0;
		auto d = std::chrono::duration_cast<std::chrono::microseconds>(tse - tsb);
		return d.count() / 1000000.0;
	}

	// return time difference in milliseconds
	double millis() const
	{
		if(tse <= tsb)
			return 0.0;
		auto d = std::chrono::duration_cast<std::chrono::microseconds>(tse - tsb);
		return d.count() / 1000.0;
	}
};

}} // ::sookee::test

namespace soo { using namespace sookee::test; }

#endif // LIBSOOKEE_TEST_H_
