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

//class Timer
//{
//	timespec tsb;
//	timespec tse;
//
//public:
//	void start() { clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb); }
//	void stop() { clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse); }
//
//	friend std::ostream& operator<<(std::ostream& o, const Timer& timer)
//	{
//		return o << (double) (timer.tse.tv_nsec - timer.tsb.tv_nsec) / 1000000000 + (double) (timer.tse.tv_sec - timer.tsb.tv_sec);
//	}
//};

class Timer
{
	const clockid_t clk = CLOCK_PROCESS_CPUTIME_ID;
	timespec tsb;
	timespec tse;
	siz iterations = 1;

public:
	Timer(clockid_t clk = CLOCK_PROCESS_CPUTIME_ID): clk(clk), tsb {0,0}, tse {0,0} {}

	void clear() { tsb = {0, 0}; tse = {0, 0}; }
	void start() { clock_gettime(clk, &tsb); }
	void stop() { clock_gettime(clk, &tse); }

	friend std::ostream& operator<<(std::ostream& o, const Timer& timer)
	{
		return o << timer.diff();
	}

	template<typename Func, typename... Args>
	double run(Func func, Args... args)
	{
		start();
		for(siz i = 0; i < iterations; ++i)
			func(args...);
		stop();

		return diff();
	}

	double diff() const { return (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec); }

//	timespec diff() const
//	{
//		timespec temp;
//		if((tse.tv_nsec - tsb.tv_nsec) < 0)
//		{
//			temp.tv_sec = tse.tv_sec - tsb.tv_sec - 1;
//			temp.tv_nsec = 1000000000 + tse.tv_nsec - tsb.tv_nsec;
//		}
//		else
//		{
//			temp.tv_sec = tse.tv_sec - tsb.tv_sec;
//			temp.tv_nsec = tse.tv_nsec - tsb.tv_nsec;
//		}
//		return temp;
//	}
//	template<typename Func, typename... Args>
//	double run(Func func, Args&&... args)
//	{
//		return run(func, args...);
//	}

	siz get_iterations() const
	{
		return iterations;
	}

	void set_iterations(siz iterations = 1)
	{
		this->iterations = iterations;
	}
};

}} // ::sookee::test

namespace soo { using namespace sookee::test; }

#endif // LIBSOOKEE_TEST_H_
