/*
 * cal.h
 *
 *  Created on: 29 Apr 2010
 *	  Author: oasookee@gmail.com
 */

#ifndef _LIBSOOKEE_CAL_H_
#define _LIBSOOKEE_CAL_H_

#include <ctime>
#include <cassert>
#include <string>
#include <limits>
#include <tuple>

namespace sookee { namespace cal {

typedef size_t year_t;
typedef size_t month_t;
typedef size_t week_t;
typedef size_t day_t;
typedef size_t hour_t;
typedef size_t min_t;
typedef size_t sec_t;

inline
year_t get_year(time_t u = time(0))
{
	return gmtime(&u)->tm_year + 1900;
}

inline
month_t get_month(time_t u = time(0))
{
	return gmtime(&u)->tm_mon;
}

inline
day_t get_date(time_t u = time(0))
{
	return gmtime(&u)->tm_mday;
}

inline std::string get_month_str(month_t m, bool full = false)
{
	static std::string mon_str_map[] =
	{
		"January"
		, "February"
		, "March"
		, "April"
		, "May"
		, "June"
		, "July"
		, "August"
		, "September"
		, "October"
		, "November"
		, "December"
	};

	return full ? mon_str_map[m] : mon_str_map[m].substr(0, 3);
}

struct date_diff_t
{
	year_t year;
	month_t month;
	week_t week;
	day_t day;
	hour_t hour;
	min_t min;
	sec_t sec;

	date_diff_t()
	: year(0), month(0), week(0), day(0), hour(0), min(0), sec(0) {}
};

inline
bool is_leap_year(year_t y)
{
	return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
}

inline
size_t days_in_month(year_t y, month_t m)
{
	static size_t dmap[] =
	{
		31		// Jan
		, 28	// Feb
		, 31	// Mar
		, 30	// Apr
		, 31	// May
		, 30	// Jun
		, 31	// Jul
		, 31	// Aug
		, 30	// Sep
		, 31	// Oct
		, 30	// Nov
		, 31	// Dec
	};

	return dmap[m] + (m == 1 ? (cal::is_leap_year(y) ? 1 : 0) : 0);
}

inline
size_t days_in_month(month_t m)
{
	return days_in_month(m, cal::get_year());
}


class date_t
: public tm
{
public:

	/**
	 * 2010-01-10
	 */
	static const char* FORMAT_ISO_8601;
	/**
	 * 20100110
	 */
	static const char* FORMAT_ISO_8601_COMPACT;
	/**
	 * 2010-01
	 */
	static const char* FORMAT_ISO_8601_MONTH;
	/**
	 * 2010-W23
	 */
	static const char* FORMAT_ISO_8601_WEEK;
	/**
	 * 2010-013 (YYYY-DDD)
	 */
	static const char* FORMAT_ISO_8601_ORDINAL;
	/**
	 * 20100110 (YYYYMMDD)
	 */
	static const char* FORMAT_STAMP_DATE;
	/**
	 * 223407 (HHMMSS)
	 */
	static const char* FORMAT_STAMP_TIME;
	/**
	 * 20100110-223407
	 */
	static const char* FORMAT_STAMP;

	enum
	{
		JAN = 0, FEB, MAR,APR ,MAY ,JUN ,JUL ,AUG ,SEP ,OCT ,NOV, DEC
	};

	enum
	{
		SUN = 0, MON, TUE, WED, THU, FRI, SAT
	};

	explicit date_t();
	explicit date_t(const date_t* date);
	explicit date_t(const date_t& date);
	explicit date_t(const time_t* time);
	explicit date_t(time_t time);
	explicit date_t(year_t y, month_t m, day_t d);
	virtual ~date_t() {}

	time_t as_unix() const { tm t = (*this); return mktime(&t); }

	bool is_leap_year()
	{
		return cal::is_leap_year(tm_year);
	}

	size_t days_in_month()
	{
		return cal::days_in_month(tm_year, tm_mon);
	}

	void dec_year()
	{
		--tm_year;
	}

	void dec_month()
	{
		if(tm_mon == JAN)
		{
			dec_year();
		}
		--tm_mon;
	}

	void dec_day()
	{
		if(tm_mday == 1)
		{
			dec_month();
			tm_mday = (int)days_in_month() + 1;
		}
		if(tm_yday == 0)
		{
			tm_yday = 365 + is_leap_year();
		}
		--tm_mday;
		--tm_yday;
	}

	date_t& operator=(const tm& d)
	{
		tm_sec = d.tm_sec;
		tm_min = d.tm_min;
		tm_hour = d.tm_hour;
		tm_mday = d.tm_mday;
		tm_mon = d.tm_mon;
		tm_year = d.tm_year;
		tm_wday = d.tm_wday;
		tm_yday = d.tm_yday;
		tm_isdst = d.tm_isdst;
		return *this;
	}

	/**
	 * %a	Abbreviated weekday name -(Thu)
	 * %A	Full weekday name -(Thursday)
	 * %b	Abbreviated month name -(Aug)
	 * %B	Full month name -(August)
	 * %c	Date and time representation -(Thu Aug 23 14:55:02 2001)
	 * %d	Day of the month (01-31) -(23)
	 * %H	Hour in 24h format (00-23) -(14)
	 * %I	Hour in 12h format (01-12) -(02)
	 * %j	Day of the year (001-366) -(235)
	 * %m	Month as a decimal number (01-12) -(08)
	 * %M	Minute (00-59) -(55)
	 * %p	AM or PM designation -(PM)
	 * %S	Second (00-61) -(02)
	 * %U	Week number with the first Sunday as the first day of week one (00-53) -(33)
	 * %w	Weekday as a decimal number with Sunday as 0 (0-6) -(4)
	 * %W	Week number with the first Monday as the first day of week one (00-53) -(34)
	 * %x	Date representation -(08/23/01)
	 * %X	Time representation -(14:55:02)
	 * %y	Year, last two digits (00-99) -(01)
	 * %Y	Year -(2001)
	 * %Z	Timezone name or abbreviation -(CDT)
	 * %%	A % sign -(%)
	 */
	std::string format(const char* fmt = 0);
};

//class Calendar
//{
//public:
//	Calendar();
//	virtual ~Calendar();
//
//	static void test(tst::context& ctx) throw(std::exception);
//};

// NEW STUFF c++11 (from public domain)

// http://home.roadrunner.com/~hinnant/date_algorithms.html

// Returns number of days since civil 1970-01-01.  Negative values indicate
//	days prior to 1970-01-01.
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//				 m is in [1, 12]
//				 d is in [1, last_day_of_month(y, m)]
//				 y is "approximately" in
//				   [numeric_limits<Int>::min()/366, numeric_limits<Int>::max()/366]
//				 Exact range of validity is:
//				 [civil_from_days(numeric_limits<Int>::min()),
//				  civil_from_days(numeric_limits<Int>::max()-719468)]

template <class Int>
/*constexpr*/ Int days_from_civil(Int y, unsigned m, unsigned d) noexcept
{
	static_assert(std::numeric_limits<unsigned>::digits >= 18,
			 "This algorithm has not been ported to a 16 bit unsigned integer");
	static_assert(std::numeric_limits<Int>::digits >= 20,
			 "This algorithm has not been ported to a 16 bit signed integer");
	y -= m <= 2;
	const Int era = (y >= 0 ? y : y-399) / 400;
	const unsigned yoe = static_cast<unsigned>(y - era * 400);		// [0, 399]
	const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;	// [0, 365]
	const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;			// [0, 146096]
	return era * 146097 + static_cast<Int>(doe) - 719468;
}

// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//				   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-719468].
template <class Int>
/*constexpr*/ std::tuple<Int, unsigned, unsigned> civil_from_days(Int z) noexcept
{
	static_assert(std::numeric_limits<unsigned>::digits >= 18,
			 "This algorithm has not been ported to a 16 bit unsigned integer");
	static_assert(std::numeric_limits<Int>::digits >= 20,
			 "This algorithm has not been ported to a 16 bit signed integer");
	z += 719468;
	const Int era = (z >= 0 ? z : z - 146096) / 146097;
	const unsigned doe = static_cast<unsigned>(z - era * 146097);			// [0, 146096]
	const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;	// [0, 399]
	const Int y = static_cast<Int>(yoe) + era * 400;
	const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);					// [0, 365]
	const unsigned mp = (5*doy + 2)/153;									// [0, 11]
	const unsigned d = doy - (153*mp+2)/5 + 1;								// [1, 31]
	const unsigned m = mp + (mp < 10 ? 3 : -9);								// [1, 12]
	return std::tuple<Int, unsigned, unsigned>(y + (m <= 2), m, d);
}

// Returns: true if y is a leap year in the civil calendar, else false
template <class Int>
constexpr bool is_leap(Int y) noexcept
{
	return  y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}


// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of common year
// The result is always in the range [28, 31].
constexpr unsigned char last_day_of_month_common_year_data[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
constexpr unsigned last_day_of_month_common_year(unsigned m) noexcept
{
	return last_day_of_month_common_year_data[m-1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of leap year
// The result is always in the range [29, 31].
constexpr unsigned char last_day_of_month_leap_year_data[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
constexpr unsigned last_day_of_month_leap_year(unsigned m) noexcept
{
	return last_day_of_month_leap_year_data[m-1];
}

// Preconditions: m is in [1, 12]
// Returns: The number of days in the month m of year y
// The result is always in the range [28, 31].
template <class Int>
constexpr unsigned last_day_of_month(Int y, unsigned m) noexcept
{
	return m != 2 || !is_leap(y) ? last_day_of_month_common_year(m) : 29u;
}

// Returns day of week in civil calendar [0, 6] -> [Sun, Sat]
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-4].
template <class Int>
constexpr unsigned weekday_from_days(Int z) noexcept
{
	return static_cast<unsigned>(z >= -4 ? (z+4) % 7 : (z+5) % 7 + 6);
}

// Preconditions: x <= 6 && y <= 6
// Returns: The number of days from the weekday y to the weekday x.
// The result is always in the range [0, 6].
constexpr unsigned weekday_difference(unsigned x, unsigned y) noexcept
{
//	x -= y;
	return (x - y) <= 6 ? (x - y) : (x - y) + 7;
}

// Preconditions: wd <= 6
// Returns: The weekday following wd
// The result is always in the range [0, 6].
constexpr unsigned next_weekday(unsigned wd) noexcept
{
	return wd < 6 ? wd+1 : 0;
}

// Preconditions: wd <= 6
// Returns: The weekday prior to wd
// The result is always in the range [0, 6].
inline constexpr unsigned prev_weekday(unsigned wd) noexcept
{
	return wd > 0 ? wd-1 : 6;
}

}} // sookee::cal

// namespace soo { using namespace sookee::cal; }

#endif /* _LIBSOOKEE_CAL_H_ */
