/*
 * test.cpp
 *
 *  Created on: Feb 4, 2013
 *      Author: oasookee@gmail.com
 */

#include <sookee/socketstream.h>

int main()
{
	soo::netstream ns;

	ns.open("localhost", "6667", std::ios::in|std::ios::out);
}


