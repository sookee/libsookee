/*
 * logrep.cpp
 *
 *  Created on: 3 Aug 2011
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com               |
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

#include <sookee/ios.h>

#include <dirent.h>
#include <sys/stat.h>

#include <sookee/types.h>
#include <sookee/bug.h>

namespace sookee { namespace ios {

using namespace sookee::bug;
using namespace sookee::types;

//sis&(&sgl)(sis&, str&, char) = std::getline;

str wordexp(const str& var, int flags)
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

str_vec wordexp_vec(const str& var, int flags)
{
	str_vec vars;

	wordexp_t p;
	if(!wordexp(var.c_str(), &p, flags))
	{
		if(p.we_wordc)
			for(char** exp = p.we_wordv; *exp; ++exp)
				vars.push_back(exp[0]);
		wordfree(&p);
	}
	return vars;
}

int wordexp_vec(const str& var, str_vec& vars, int flags)
{
	wordexp_t p;
	if(int err = wordexp(var.c_str(), &p, flags) != 0)
		return err;

	vars.clear();
	if(p.we_wordc)
		for(char** exp = p.we_wordv; *exp; ++exp)
			vars.push_back(exp[0]);

	wordfree(&p);

	return 0;
}

/**
 * Read past whitespace then read in a string up to the
 * nect space ' ' character or, if enclosed in single
 * or double quotes '',"" read up to the terminating quite.
 *
 * The quotes are not included in the returned string.
 * @param is
 * @param s
 * @return
 */
std::istream& getstring(std::istream& is, str& s)
{
	char term = ' ';

	if(is.flags() & is.skipws)
		is >> std::ws;

	if(is.peek() == '"' || is.peek() == '\'')
		is.get(term);

	char c;
	str n;
	while(is.get(c) && c != term)
		n.append(1, c);

	if(n.empty())
		is.setstate(std::ios::failbit);
	else
	{
		s = n;
		is.clear();
	}

	return is;
}

std::istream& getnested(std::istream& is, str& s, char d1, char d2)
{
	using std::ws;

	if(is.peek() != d1)
	{
		is.setstate(std::ios::failbit);
		return is;
	}

	str n;
	uns d = 0;
	char c;
	while(is.get(c))
	{
		n.append(1, c);
		if(c == d1)
			++d;
		else if(c == d2)
			--d;
		if(!d)
			break;
	}

	if(n.empty())
		is.setstate(std::ios::failbit);
	else
	{
		s = n;
		is.clear();
	}

	return is;
}

int ls(const str& folder, str_vec &files)
{
    DIR* dir;
	dirent* dirp;

	if(!(dir = opendir(folder.c_str())))
		return errno;

	files.clear();
	while((dirp = readdir(dir)))
		files.push_back(dirp->d_name);

	if(closedir(dir))
		return errno;

	return 0;
}

bool ls(const str& folder, str_vec& folders, str_vec &files)
{
    DIR* dir;
	dirent* dirp;

	if(!(dir = opendir(folder.c_str())))
		return false;

	files.clear();
	while((dirp = readdir(dir)))
	{
		if(dirp->d_type == DT_DIR)
			folders.push_back(dirp->d_name);
		else if(dirp->d_type == DT_REG)
			files.push_back(dirp->d_name);
		else if(dirp->d_type == DT_UNKNOWN)
		{
			struct stat s;
			if(!stat((folder + "/" + dirp->d_name).c_str(), &s))
			{
				if(S_ISDIR(s.st_mode))
					folders.push_back(dirp->d_name);
				else if(S_ISREG(s.st_mode))
					files.push_back(dirp->d_name);
			}
		}
	}
	if(closedir(dir))
		return false;

	return true;
}

str_vec ls(const str& folder, ftype t)
{
    DIR* dir;
	dirent* dirp;
	str_vec files;

	if(!(dir = opendir(folder.c_str())))
		return files;

	struct stat s;

	while((dirp = readdir(dir)))
	{
		if(!stat((folder + "/" + dirp->d_name).c_str(), &s))
		{
			if((t == ftype::dir && S_ISDIR(s.st_mode))
			|| (t == ftype::reg && S_ISREG(s.st_mode))
			|| (t == ftype::chr && S_ISCHR(s.st_mode))
			|| (t == ftype::blk && S_ISBLK(s.st_mode))
			|| (t == ftype::fifo && S_ISFIFO(s.st_mode))
			|| (t == ftype::lnk && S_ISLNK(s.st_mode))
			|| (t == ftype::sock && S_ISSOCK(s.st_mode))
			|| (t == ftype::all))
				files.push_back(dirp->d_name);
		}
	}

	closedir(dir);

	return files;
}


}} // sookee::ios
