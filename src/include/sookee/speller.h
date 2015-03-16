/*
 *  Created on: Mar 15, 2015
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_SPELLER_H_
#define LIBSOOKEE_SPELLER_H_

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

// For now header only to avoid dependency checks
// in build
// TODO: add conditional build of libsookee-aspell.so
//

#include <aspell.h>

#include <sookee/types/basic.h>
#include <sookee/types/str_vec.h>

namespace sookee { namespace spell {

using namespace sookee::types;

class ASpeller
{
	AspellConfig* config = nullptr;
	AspellSpeller* speller = nullptr;
	int err = 0;

public:
	str error;

	ASpeller(const str& lang = "en");
	~ASpeller();

	bool correct(const str& word);
	str_vec suggest(const str& word);

//	str_vec get_infos();
};

/**
 * Example usage
 */
//int main()
//{
//	ASpeller speller;
//
//	str line;
//	while(std::getline(std::cin, line))
//	{
//		if(speller.correct(line))
//			std::cout << "speller: " << line << " is spelled correctly." << '\n';
//		else
//		{
//			for(auto&& word: speller.suggest(line))
//				std::cout << "speller: " << word << '\n';
//		}
//
//	}
//
//	return 0;
//}

}} // ::sookee::spell

namespace soo { using namespace sookee::spell; }

#endif // LIBSOOKEE_SPELLER_H_
