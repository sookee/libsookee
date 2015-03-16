/*
 *  Created on: Mar 16, 2015
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_ARGS_H_
#define LIBSOOKEE_ARGS_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee <oasookee@gmail.com>				   |
'------------------------------------------------------------------'

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

http://www.gnu.org/licenses/gpl-2.0.html

'-----------------------------------------------------------------*/

//#include <SFML/Graphics.hpp>

#include <sookee/types/basic.h>
#include <sookee/types/stream.h>

namespace sookee { namespace args {

using namespace sookee::types;

class Args
{
public:
	typedef char const* const* arg_ptr;
	std::string error;

private:
	arg_ptr arg;

public:
	Args(arg_ptr arg): arg(arg) {}

	bool has_next() const { return *arg; }
	void next() { ++arg; }

//	arg_ptr begin() { return arg; }
//	arg_ptr end() { arg_ptr e = arg; while(*e) ++e; return e; }

	bool test_arg(const std::string& opt)
	{
		return opt == *arg;
	}

	template<typename Type>
	bool get_arg(const std::string& opt, Type& out)
	{
		if(test_arg(opt))
		{
			if(!*(++arg))
			{
				error = "option " + opt + " requires a parameter.";
				return false;
			}
			if(!(std::istringstream(*arg) >> std::boolalpha >> out))
			{
				error = "bad parameter to option " + opt + ".";
				return false;
			}
		}
		return true;
	}

	bool test_arg(const std::string& lopt, const std::string& sopt)
	{
		return test_arg(lopt) || test_arg(sopt);
	}

	template<typename Type>
	bool get_arg(const std::string& lopt, const std::string& sopt, Type& out)
	{
		if(test_arg(lopt))
			return get_arg(lopt, out);
		else if(test_arg(sopt))
			return get_arg(sopt, out);
		return true;
	}
};

//Args::arg_ptr begin(Args& args) { return args.begin(); }
//Args::arg_ptr end(Args& args) { return args.end(); }

/**
 * Example usage
 */
//int main(int argc, char* argv[])
//{
//	std::string s;
//	std::istringstream("hello") >> s;
//
//	(void) argc;
//	std::string lang = "en";
//
//	bool test1 = false;
//	int test2 = -1;
//	unsigned test3 = 1U;
//
//
//	for(Args args(argv); args.has_next(); args.next())
//	{
//		if(args.test_arg("--help", "-h"))
//		{
//			usage(argv[0]);
//			return 0;
//		}
//		else if(!args.get_arg("--bool", "-b", test1))
//		{
//			con("ERROR: " << args.error);
//		}
//		else if(!args.get_arg("--int", "-i", test2))
//		{
//			con("ERROR: " << args.error);
//		}
//		else if(!args.get_arg("--unsigned", "-u", test3))
//		{
//			con("ERROR: " << args.error);
//		}
//		else if(!args.get_arg("--lang", "-l", lang))
//		{
//			con("ERROR: " << args.error);
//			usage(argv[0]);
//			return 1;
//		}
//	}
//}

}} // ::sookee::args

namespace soo { using namespace sookee::args; }

#endif // LIBSOOKEE_ARGS_H_
