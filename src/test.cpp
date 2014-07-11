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

using namespace soo;

int main()
{
	ADD_STACK_HANDLER();

	con("Hello world");

	str_vec v(2);

	for(siz i = 0; i < 10; ++i)
		v[i] = 10;

	con("Goodbye cruel world");
}
