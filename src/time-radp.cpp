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

#include <ctime>

using namespace soo;

int main()
{
	timespec tsb;
	timespec tse;

	str t1 = "-10245.00349";

	rad pos = t1.data();
	float v;

	con("radp::pf()");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < 100000000; ++i)
		pf(pos, v);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	double diff = (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	bug_var(diff);

	con("std::sscanf()");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < 100000000; ++i)
		std::sscanf(pos, "%f", &v);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	diff = (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	bug_var(diff);

	con("std::stod()");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < 100000000; ++i)
		v = std::stod(t1);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	diff = (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	bug_var(diff);

	con("std::atof()");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < 100000000; ++i)
		v = std::atof(pos);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	diff = (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	bug_var(diff);

	con("std::strtod()");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsb);
	for(siz i = 0; i < 100000000; ++i)
		v = std::strtod(pos, 0);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tse);

	diff = (double) (tse.tv_nsec - tsb.tv_nsec) / 1000000000 + (double) (tse.tv_sec - tsb.tv_sec);
	bug_var(diff);
}
