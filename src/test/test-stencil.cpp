/*
 *  Created on: 29 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/stencil.h>
#include <sookee/test.h>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::test;
using namespace sookee::utils;

const str_vec arg_tests =
{
	""
	, "${1}"
	, "${ 1}"
	, "${1 }"
	, "${ 1 }"
	, "${1}${2}"
	, "${1} ${2}"
	, "X${  1  }"
	, "${1}X"
	, "X${1}${2}"
	, "${1}X${2}"
	, "${1}${2}X"
	, "X${1}X${2}"
	, "${1}X${2}X"
};

const str_vec map_tests =
{
	"start"
	, "${v1}"
	, "start${v1}"
	, "${v1}end"
	, "start${v1}end"
	, "start${v1}mid1${v2}mid2${v1}end"
	, "start${v1}mid1${v2}mid2${v1}end"
};

const str timer_test = R"(
0123456789 ${1} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${2} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${3} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${4} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${5} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${6} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${7} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${8} ABCDEFGHIJKLMNOPQRSTUVWXYZ
)";

int main()
{
	for(const auto& tmp: arg_tests)
	{
		arg_stencil tplt;
		tplt.compile(tmp);
//		tplt.dump();
		con("-----------------------------------------------");
		con("tmp: " << tmp);
		con("   : " << tplt.create("ARG1", "LONGARG2"));
	}

	map_stencil::dict d {{"v1", "s1"}, {"v2", "s2"}};

	map_stencil s;

	for(auto&& t: map_tests)
	{
		s.compile(t);
//		s.dump();
		con(s.create(d));
		con("");
	}

	const uns repeats = 1000000;

	Timer timer;

	arg_stencil as;
	map_stencil ms;
	str ar, mr;

	const str_vec subs = {"one", "two", "three", "four", "five", "six", "seven", "eight"};

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		as.compile(timer_test);
		ar = as.create(subs[0], subs[1], subs[2], subs[3], subs[4], subs[5], subs[6], subs[7]);
	}
	timer.stop();

	con(ar);
	con(timer);

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		ms.compile(timer_test);
		mr = ms.create({{"1", subs[0]}, {"2", subs[1]}, {"3", subs[2]}, {"4", subs[3]}
			, {"5", subs[4]}, {"6", subs[5]}, {"7", subs[6]}, {"8", subs[7]}});
	}
	timer.stop();

	con(mr);
	con(timer);

	map_stencil::dict d2 {{"1", subs[0]}, {"2", subs[1]}, {"3", subs[2]}, {"4", subs[3]}
	, {"5", subs[4]}, {"6", subs[5]}, {"7", subs[6]}, {"8", subs[7]}};

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		ms.compile(timer_test);
		mr = ms.create(d2);
	}
	timer.stop();

	con(mr);
	con(timer);
}







