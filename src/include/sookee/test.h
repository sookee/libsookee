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
#include <iomanip>

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
	struct clock_type {};
	struct precision_type {};
	template<typename T, typename ID>
	struct helper
	{
		ID id;
		T p;
		helper(T p = {}): p(p) {}
		operator T() const { return p; }
	};
public:

	using clock = helper<decltype(CLOCK_MONOTONIC), clock_type>;
	using precision = helper<unsigned, precision_type>;

private:
	decltype(CLOCK_MONOTONIC) clk;
	timespec tsb;
	timespec tse;
	precision p = 6;

public:
	PosixTimer()
	: PosixTimer(clock(CLOCK_MONOTONIC), precision(6)) {}

	explicit PosixTimer(clock clk)
	: PosixTimer(clk, precision(6)) {}

	explicit PosixTimer(precision p)
	: PosixTimer(clock(CLOCK_MONOTONIC), p) {}

	explicit PosixTimer(clock clk, precision p)
	: clk(clk), p(p) {}

	void clear() { start(); tse = tsb; }
	void start() { clock_gettime(clk, &tsb); }
	void stop() { clock_gettime(clk, &tse); }

	friend std::ostream& operator<<(std::ostream& o, const PosixTimer& timer)
	{
		return o << std::fixed << std::setprecision(timer.p) << timer.secs();
	}

	auto nanos() const
	{
		auto b = (tsb.tv_sec * 1000000000) + tsb.tv_nsec;
		auto e = (tse.tv_sec * 1000000000) + tse.tv_nsec;
		return e - b;
	}

	double millis() const
	{
		return nanos() / 1000000.0;
	}

	double secs() const
	{
		return nanos() / 1000000000.0;
	}
};

class Timer
{
	using clk = std::chrono::steady_clock;
	clk::time_point tsb;
	clk::time_point tse;

public:

	void clear() { tsb = tse = clk::now(); }
	void start() { tsb = clk::now(); }
	void stop() { tse = clk::now(); }

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
