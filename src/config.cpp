/*
 * Author: SooKee <oasookee@gmail.com>
 *
 * Created on Jul 26, 2014
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee <oasookee@gmail.com>                   |
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

#include <sookee/config.h>
#include <wordexp.h>

namespace sookee { namespace props {

using namespace sookee::types;

str Config::expand_env(const str& var, int flags)
{
	str exp = var;
	wordexp_t p;
	if(!wordexp(var.c_str(), &p, flags))
	{
		if(p.we_wordc && p.we_wordv[0])
			exp = p.we_wordv[0];
		wordfree(&p);
	}
	return exp;
}

bool Config::load(const str& file, bool first)
{
	static str_set includes;

	if(first)
		includes.clear();

	str config_file = dir + "/" + expand_env(file);

	if(!includes.insert(config_file).second)
		return true; // endless recursion

	log("PROPS LOAD: " << config_file);

	std::ifstream ifs(config_file.c_str());

	if(!ifs.is_open())
	{
		log("ERROR: opening config file");
		return false;
	}

	// working variables
	siz pos;
	str line, key, val;

	// read in config

	siz no = 0;
	while(sgl(ifs, line))
	{
		++no;

		if((pos = line.find("//")) != str::npos)
			line.erase(pos);

		trim(line);

		if(line.empty() || line[0] == '#')
			continue;

		siss iss(line);
		if(!sgl(sgl(iss, key, ':') >> std::ws, val))
		{
			log("ERROR: parsing config file: " << file << " at: " << no);
			log("ERROR:					: " << line);
			continue;
		}

		if(key == "include")
			load(val, false);
		else
			props[key].push_back(val);
	}

	ifs.close();
	log("PROPS LOAD: OK:");

	return true;
}


}} // ::sookee::props
