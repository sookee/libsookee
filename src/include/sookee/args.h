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

	Args(const Args& args): error(args.error), arg(args.arg) {}
private:
	arg_ptr arg;

public:
	Args(arg_ptr arg): arg(arg) {}

	operator bool() const { return *arg; }
	Args& operator++() { ++arg; return *this; }
	Args operator++(int) { Args arg(*this); ++arg; return arg; }

	// Experimental untested API (self-iterator)
	Args begin() { return *this; }
	Args end() { arg_ptr end = arg; while(*end++){} return Args(end); }
	Args& operator*() { return *this; }
	bool operator==(const Args& args) const { return arg == args.arg; }
	// experimental

	bool test(const std::string& opt)
	{
		return opt == *arg;
	}

	template<typename Type>
	bool get(const std::string& opt, Type& out)
	{
		if(test(opt))
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

	bool test(const std::string& lopt, const std::string& sopt)
	{
		return test(lopt) || test(sopt);
	}

	template<typename Type>
	bool get(const std::string& lopt, const std::string& sopt, Type& out)
	{
		if(test(lopt))
			return get(lopt, out);
		else if(test(sopt))
			return get(sopt, out);
		return true;
	}
};

/**
 * Example usage
 */
//int main(int argc, char* argv[])
//{
//	(void) argc;
//	std::string lang = "en";
//
//	for(Args args(argv); args; ++args)
//	{
//		if(args.test("--help", "-h"))
//		{
//			usage(argv[0]);
//			return 0;
//		}
//		else if(!args.get("--lang", "-l", lang))
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
