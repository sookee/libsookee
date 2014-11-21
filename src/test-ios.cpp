/*
 *  Created on: 09 Oct 2014
 *	  Author: oasookee@gmail.com
 */

#include <sookee/ios.h>
#include <sookee/log.h>

using namespace sookee;
using namespace sookee::log;
using namespace sookee::types;

int main()
{
	using std::ws;

	siss iss("   {aaaa{bb}ccc}ddddd}");

	str s;
	if(ios::getnested(iss >> ws, s))
	{
		con(s);
	}

//	int count;
//
//	if(!ios::user_insist(std::cin, std::cout, count, "Number of items: "
//		, "Unrecognized input!", 3))
//	{
//		std::cout << "Too many errors!" << '\n';
//		return 1;
//	}
//
//	std::cout << count << '\n';
//
//	while(count--)
//	{
//
//	}
}
