/*
 *  Created on: 07 Oct 2014
 *	  Author: oasookee@gmail.com
 */

#include <sookee/stl.h>
#include <sookee/log.h>
#include <sookee/types/vec.h>

#include <iostream>

using namespace sookee;
using namespace sookee::log;
using namespace sookee::types;

std::ostream& operator<<(std::ostream& os, const int_vec& v)
{
	os << "{";
	str sep;
	for(const auto& i: v)
		{ os << sep << i; sep = ", "; }
	os << "}";
	return os;
}

int main()
{
	typedef std::vector<int> int_vec;
	typedef std::map<std::string, std::string> str_map;
	typedef str_map::value_type str_map_vt;

	std::srand((unsigned)std::time(0));

	int_vec ints {1, 4, 2, 3, 5, 4};
	str_map strs {{"a", "1"}, {"b", "2"}, {"c", "3"}};

	const int_vec cints {1, 4, 2, 3, 5, 4};
	const str_map cstrs {{"a", "1"}, {"b", "2"}, {"c", "3"}};

	con("count  1: " << stl::count(ints, 4));
	con("count  2: " << stl::count(strs, {"b", "2"}));

	con("count  3: " << stl::count_if(ints, [](int i){return i & 1;}));
	con("count  4: " << stl::count_if(strs, [](const str_map_vt& v){return v.second <= "2";}));

	con("count  5: " << stl::count_key(strs, "b"));

	con("count  6: " << stl::count(cints, 4));
	con("count  7: " << stl::count(cstrs, {"b", "2"}));

	con("count  8: " << stl::count_if(cints, [](int i){return i & 1;}));
	con("count  9: " << stl::count_if(cstrs, [](const str_map_vt& v){return v.second <= "2";}));

	con("count 10: " << stl::count_key(cstrs, "b"));

	con("find  1: " << *stl::find(ints, 4));
	con("find  2: " << stl::find(strs, {"b", "2"})->first);

	con("find  3: " << *stl::find_if(ints, [](int i){return i & 1;}));
	con("find  4: " << stl::find_if(strs, [](const str_map_vt& v){return v.second <= "2";})->first);

	con("count 5: " << stl::find_key(strs, "b")->second);

	con("find  6: " << *stl::find(cints, 4));
	con("find  7: " << stl::find(cstrs, {"b", "2"})->first);

	con("find  8: " << *stl::find_if(cints, [](int i){return i & 1;}));
	con("find  9: " << stl::find_if(cstrs, [](const str_map_vt& v){return v.second <= "2";})->first);

	con("find 10: " << stl::find_key(strs, "b")->second);

//	con("find 2: " << *(stl::find_if(cints, [](int i){return i & 1;})));
//
//	con("find 3: " << (*(stl::find(ints, 4)) = 9));
//	con("find 4: " << (*(stl::find_if(ints, [](int i){return i & 1;})) = 11));

	stl::sort(ints);
	con("sort 1: " << ints);
	stl::sort(ints, [](int a, int b){return (a^0xff) < (b^0xff);});
	con("sort 2: " << ints);
	stl::random_shuffle(ints);
	con("sort 3: " << ints);
}
