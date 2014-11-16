/*
 * test.cpp
 *
 *  Created on: 23 Apr 2014
 *      Author: oasookee@gmail.com
 */

#include <sookee/types.h>
#include <sookee/str.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/radp.h>
#include <sookee/test.h>

#include <ctime>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::test;
using namespace sookee::radp;
using namespace sookee::types;
using namespace sookee::utils;

int main()
{
	Timer timer;
	str t1 = "-10245.00349";

	rad pos = t1.data();
	float v;

	con("radp::pf()");
	timer.start();
	for(siz i = 0; i < 100000000; ++i)
		pf(pos, v);
	timer.stop();

	bug_var(timer);

	con("std::sscanf()");
	timer.start();
	for(siz i = 0; i < 100000000; ++i)
		std::sscanf(pos, "%f", &v);
	timer.stop();

	bug_var(timer);

	con("std::stod()");
	timer.start();
	for(siz i = 0; i < 100000000; ++i)
		v = std::stod(t1);
	timer.stop();

	bug_var(timer);

	con("std::atof()");
	timer.start();
	for(siz i = 0; i < 100000000; ++i)
		v = std::atof(pos);
	timer.stop();

	bug_var(timer);

	con("std::strtod()");
	timer.start();
	for(siz i = 0; i < 100000000; ++i)
		v = std::strtod(pos, 0);
	timer.stop();

	bug_var(timer);
}
