/*
 * Author: SooKee <oasookee@gmail.com>
 *
 * Created on Mar 16, 2015
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee <oasookee@gmail.com>                   |
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

#include <sookee/speller.h>

namespace sookee { namespace spell {

ASpeller::ASpeller(const str& lang)
: config(new_aspell_config())
{
	aspell_config_replace(config, "lang", lang.c_str());

	AspellCanHaveError* ret = nullptr;

	if(aspell_error_number(ret = new_aspell_speller(config)))
		delete_aspell_config(config);
	else
	{
		speller = to_aspell_speller(ret);
	}
}

ASpeller::~ASpeller()
{
	delete_aspell_config(config);
}

bool ASpeller::correct(const str& word)
{
	if((err = aspell_speller_check(speller, word.c_str(), word.size())) == -1)
	{
		error = aspell_speller_error(speller)->mesg;
		return false;
	}
	return err;
}

str_vec ASpeller::suggest(const str& word)
{
	auto list = aspell_speller_suggest(speller, word.c_str(), word.size());
	auto iter = aspell_word_list_elements(list);

	str_vec v;
	v.reserve(aspell_word_list_size(list));

	while(!aspell_string_enumeration_at_end(iter))
		v.emplace_back(aspell_string_enumeration_next(iter));

	delete_aspell_string_enumeration(iter);

	return v;
}

//	str_vec ASpeller::get_infos()
//	{
//		auto list = get_aspell_dict_info_list(config);
//		auto iter = aspell_dict_info_list_elements(list);
//
//		str_vec v;
//		v.reserve(aspell_dict_info_list_size(list));
//
//		while(!aspell_dict_info_enumeration_at_end(iter))
//		{
//			auto info = aspell_dict_info_enumeration_next(iter);
//			info->
//		}
//		delete_aspell_dict_info_enumeration(iter);
//
//		return v;
//	}


}} // ::sookee::spell
