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
	bug_var(std::numeric_limits<siz>::max());
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
		ints.emplace_back(ipick(eng, irange));
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

//	flt_vec floats;

	for(siz i = 0; i < max_floats; ++i)
	{
//		floats.emplace_back(rpick(eng, rrange));
		text += sep + std::to_string(rpick(eng, rrange));
		sep = " ";
	}

	bug_var(text);
	bug_var(std::numeric_limits<int>::min());
	bug_var(std::numeric_limits<int>::max());
	//return 1;

	pos = text.data();
	err = 0;
	for(siz i = 0; i < max_floats; ++i)
	{
		float v, vv;
		sscanf(pos, "%f", &vv);
		if((end = pf(pos, v)) == pos)
		{
			log("ERROR: parse failed");
			++err;
		}
		else if(v != vv)//(int(v * 10000000000) != int(vv * 10000000000))
		{
			bug_var(pos);
			bug_var(int(v * 10000000000));
			bug_var(int(vv * 10000000000));
			log("ERROR: parsed value (" << v << ") != expected value: " << vv);
			++err;
		}
		pos = adv(end, 1);
	}

	con("\t\t\t" << (err ? "[FAIL]" : "[PASS]"));

	con("End of testing:");
}
