/*
 *  Created on: 29 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/stencil.h>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::string;

int main()
{
	str_vec tmps =
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

	for(const auto& tmp: tmps)
	{
		stencil tplt;
		tplt.compile(tmp);
//		tplt.dump();
		con("-----------------------------------------------");
		con("tmp: " << tmp);
		con("   : " << tplt.create("ARG1", "LONGARG2"));
	}
}
