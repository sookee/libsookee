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

using sookee::types;

class Timer
{
	timespec tsb;
	timespec tse;
	siz iterations = 1;

public:

	template<typename Func, typename... Args>
	double run(Func func, Args&... args)
	{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
		for(siz i = 0; i < iterations; ++i)
			func(args...);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

		return (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	}

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
