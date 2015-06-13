/*
 *  Created on: 29 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/types/vec.h>
#include <sookee/str.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/rnd.h>

#include <sookee/radp.h>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::rnd;
using namespace sookee::radp;
using namespace sookee::types;
using namespace sookee::utils;

static const siz max_ints = 1000;
static const siz max_floats = 1000;

int main()
{
//	bug_var(std::numeric_limits<siz>::max());
	con("Testing radp");
	std::random_device rd;
	std::default_random_engine eng{rd()};
	std::uniform_int_distribution<> ipick;
	std::uniform_real_distribution<> rpick;
//	int n = pick(eng, decltype(pick)::param_type{0, A});

	str sep;
	str text;
	auto irange = decltype(ipick)::param_type{0, 0};
	auto rrange = decltype(rpick)::param_type{0., 0.};

	con_e();
	con_s("Parsing random ints:");

	irange = decltype(ipick)::param_type{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};

	rad pos;
	rad end;
	sep.clear();
	text.clear();

	int_vec ints;

	for(siz i = 0; i < max_ints; ++i)
	{
		ints.push_back(ipick(eng, irange));
		text += sep + std::to_string(ints[i]);
		sep = " ";
	}

	//bug_var(text);

	pos = text.data();
	siz err = 0;
	for(siz i = 0; i < max_ints; ++i)
	{
		int v;
		if((end = pi(pos, v)) == pos)
		{
			log("ERROR: parse failed");
			++err;
		}
		else if(v != ints[i])
		{
			log("ERROR: parsed value (" << v << ") != expected value: " << ints[i]);
			++err;
		}
		pos = adv(end, 1);
	}

	con("\t\t\t" << (err ? "[FAIL]" : "[PASS]"));

	con_e();
	con_s("Parsing random floats:");

	rrange = decltype(rpick)::param_type{-100.0f, 100.0f};

	sep.clear();
	text.clear();

	flt_vec floats;

	for(siz i = 0; i < max_floats; ++i)
	{
//		siz dp = std::pow(10, ipick(eng, decltype(ipick)::param_type{0, 5}));
//		floats.push_back(((int)rpick(eng, rrange) * dp)/float(dp));
		floats.push_back(rpick(eng, rrange));
		text += sep + std::to_string(floats[i]);
		sep = " ";
	}

	pos = text.data();
	err = 0;
	for(siz i = 0; i < max_floats; ++i)
	{
		float v;//, vv;
//		sscanf(pos, "%f", &vv);
		if((end = pf(pos, v)) == pos)
		{
			log("ERROR: parse failed");
			++err;
		}
		else if(v != floats[i])//vv)
		{
			log("ERROR: parsed value (" << v << ") != expected value: " << floats[i]);//vv);
			++err;
		}
		pos = adv(end, 1);
	}

	con("\t\t\t" << (err ? "[FAIL]" : "[PASS]"));

	con("Parsing literals:");

	const str_vec literals {"", "a", "ab", "abc"};
	const str_vec lit_data {"", "a", "ab", "abc", "abcd", "abcde", "f"};
	const int_vec lit_fix
	{
		  0, 0, 0, 0
		, 0, 1, 0, 0
		, 0, 1, 2, 0
		, 0, 1, 2, 3
		, 0, 1, 2, 3
		, 0, 1, 2, 3
		, 0, 0, 0, 0
	};

	auto fix = lit_fix.begin();

	for(auto&& data: lit_data)
	{
		for(auto&& lit: literals)
		{
			con_s(": \"" << data << "\", \"" << lit << "\": ");
			rad s = pl(data.data(), lit.data());
			con("\t\t\t\t" << ((*fix != s - data.data()) ? "[FAIL]" : "[PASS]"));
			++fix;
		}
	}

	con("End of testing:");
}
