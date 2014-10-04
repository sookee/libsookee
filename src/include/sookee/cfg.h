/*
 *  Created on: Jul 26, 2014
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_CONFIG_H_
#define LIBSOOKEE_CONFIG_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee <oasookee@gmail.com>				   |
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

#include <sookee/str.h>
#include <sookee/types/typedefs_map.h>
#include <sookee/log.h>
#include <wordexp.h>

namespace sookee { namespace props {

using namespace sookee;
using namespace sookee::types;
using namespace sookee::string;

class Config
{
private:
	TYPEDEF_MAP(str, str_vec, property_map);
	property_map props;

	/**
	 * Location of the configuration folder.
	 * Typically something like $HOME/.prog
	 */
	str dir;
	bool load(const str& dir, const str& file, bool first);

	str expand_env(const str& var, int flags = 0) const;

public:

	bool load(const str& dir, const str& file) { return load(dir, file, true); }

	/**
	 * Get a type-converted config variable's value.
	 *
	 * @param s The config variable whose value is sought.
	 * @param dflt A default value to use if the variable was not
	 * found in the config file.
	 *
	 * @return The value that the variable s is set to in the config
	 * file else dflt if not present.
	 */
	template<typename T>
	T get(const str& s, const T& dflt = T()) const
	{
		const auto found = props.find(s);

		if(found == props.end())
			return dflt;

		T t;
		std::istringstream(*(found->second.begin())) >> std::boolalpha >> t;
		return t;
	}

	/**
	 * Get the native string config variable's value.
	 *
	 * @param s The config variable whose value is sought.
	 * @param dflt A default value to use if the variable was not
	 * found in the config file.
	 *
	 * @return The value that the variable s is set to in the config
	 * file else dflt if not present.
	 */
	str get(const str& s, const str& dflt = "") const
	{
		const auto found = props.find(s);
		return found != props.end() ? *(found->second.begin()) : dflt;
	}

	/**
	 * Get a file-path converted config variable's value.
	 *
	 * @param s The config variable whose value is sought.
	 * @param dflt A default value to use if the variable was not
	 * found in the config file.
	 *
	 * @return The value that the variable s is set to in the config
	 * file after file glob explnsion has been applied else dflt if
	 * not present.
	 */
	str get_exp(const str& s, const str& dflt = "") const
	{
		const auto found = props.find(s);
		return found != props.end() ? expand_env(*(found->second.begin()), WRDE_SHOWERR|WRDE_UNDEF) : dflt;
	}

	/**
	 * Get the native string vector config variable's values
	 * when the same variable is provided several times in the
	 * config file.
	 *
	 * @param s The config variable whose values are sought.
	 *
	 * @return All the values that the variable s is set to in the config
	 * file as a vector of strings (str_vec). May be empty if variable is
	 * not present.
	 */
	const str_vec& get_vec(const str& s) const
	{
		static const str_vec null_vec;

		const auto found = props.find(s);

		if(found == props.end())
			return null_vec;

		return found->second;
	}

	/**
	 * Get the native string vector config variable's file-name
	 * exanded values when the same variable is provided several
	 * times in the config file.
	 *
	 * @param s The config variable whose values are sought.
	 *
	 * @return All the file-name expanded  values that the
	 * variable s is set to in the config file as a vector
	 * of strings (str_vec). May be empty if variable is
	 * not present.
	 */
	str_vec get_exp_vec(const str& s) const
	{
		str_vec v = get_vec(s);
		for(siz i = 0; i < v.size(); ++i)
			v[i] = expand_env(v[i], WRDE_SHOWERR|WRDE_UNDEF);
		return v;
	}

	/**
	 * Check if the config file has a given variable set.
	 */
	bool has(const str& s) const
	{
		return props.find(s) != props.end();// && !props[s].empty();
	}

	/**
	 * Synonym for bool has(const str& s).
	 */
	bool have(const str& s) const { return has(s); }
};

}} // ::sookee::props

namespace soo { using namespace sookee::props; }

#endif // LIBSOOKEE_CONFIG_H_
