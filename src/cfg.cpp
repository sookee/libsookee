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

#include <sookee/cfg.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/ios.h>
#include <sookee/types/str_set.h>
#include <sookee/str.h>
#include <wordexp.h>

namespace sookee { namespace props {

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::ios;
using namespace sookee::types;
using namespace sookee::utils;

bool Config::load(const str& dir, const str& file, bool first)
{
	static str_set includes;

	if(first)
	{
		this->dir = dir;
		includes.clear();
		props.clear();
	}
	str config_file = wordexp(dir) + "/" + wordexp(file);

	if(!includes.insert(config_file).second)
		return true; // endless recursion

	log("PROPS LOAD: " << config_file);

	std::ifstream ifs(config_file.c_str());

	if(!ifs.is_open())
	{
		log("E: opening config file");
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
			log("E: parsing config file: " << file << " at: " << no);
			log(" :	" << line);
			continue;
		}

		if(key == "include")
			load(dir,val, false);
		else
		{
			bug("CFG: " << key << ": " << val);
			props[key].push_back(val);
		}
	}

	ifs.close();
	log("PROPS LOAD: OK:");

	return true;
}

}} // ::sookee::props
