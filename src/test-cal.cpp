/*
 *  Created on: 20 Aug 2014
 *	  Author: public domain
 */

#include <sookee/cal.h>

#include <iostream>
#include <chrono>
#include <cassert>

using namespace sookee::cal;

//static_assert(days_from_civil(1970, 1, 1) == 0, "1970-01-01 is day 0");
//static_assert(civil_from_days(0) == std::make_tuple(1970, 1, 1), "1970-01-01 is day 0");
//static_assert(weekday_from_days(days_from_civil(1970, 1, 1)) == 4, "1970-01-01 is a Thursday");

int main()
{
	assert(days_from_civil(1970, 1, 1) == 0);
	assert(civil_from_days(0) == std::make_tuple(1970, 1, 1));
	assert(weekday_from_days(days_from_civil(1970, 1, 1)) == 4);

	int ystart = -1000000;
	int prev_z = days_from_civil(ystart, 1, 1) - 1;
	assert(prev_z < 0);
	unsigned prev_wd = weekday_from_days(prev_z);
	assert(prev_wd <= 6);
	auto t0 = std::chrono::system_clock::now();
	for (int y = ystart; y <= -ystart; ++y)
	{
		for (unsigned m = 1; m <= 12; ++m)
		{
			unsigned e = last_day_of_month(y, m);
			for (unsigned d = 1; d <= e; ++d)
			{
				int z = days_from_civil(y, m, d);
				assert(prev_z < z);
				assert(z == prev_z+1);
				int yp;
				unsigned mp, dp;
				std::tie(yp, mp, dp) = civil_from_days(z);
				assert(y == yp);
				assert(m == mp);
				assert(d == dp);
				unsigned wd = weekday_from_days(z);
				assert(wd <= 6);
				assert(wd == next_weekday(prev_wd));
				assert(prev_wd == prev_weekday(wd));
				prev_z = z;
				prev_wd = wd;
			}
		}
	}
	auto t1 = std::chrono::system_clock::now();
	typedef std::chrono::duration<float> sec;
	std::cout << sec(t1-t0).count() << '\n';
	std::cout << days_from_civil(1000000L, 12, 31) - days_from_civil(-1000000L, 1, 1) << '\n';
}
