#pragma once
#ifndef _LIBSOOKEE_IOS_H_
#define _LIBSOOKEE_IOS_H_
/*
 * ios.h
 *
 *  Created on: 28 Jan 2012
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oasookee@gmail.com               |
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

#include <sookee/types.h>

#include <istream>
#include <ostream>

#include <dirent.h>
#include <sys/stat.h>

namespace sookee { namespace ios {

using namespace sookee::types;

std::istream& getstring(std::istream& is, str& s);
inline
std::istream& getstring(std::istream&& is, str& s) { return getstring(is, s); }

/**
 * Get directory listing.
 * @return errno
 */
inline
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

/**
 * Get directory listing.
 * @return errno
 */
inline
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
			if(!stat(dirp->d_name, &s))
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

enum class ftype
{
	unk, reg, dir, chr, blk, fifo, lnk, sock, all
};

inline
str_vec ls(const str& folder, ftype t = ftype::reg)
{
    DIR* dir;
	dirent* dirp;
	str_vec files;

	if(!(dir = opendir(folder.c_str())))
		return files;

	struct stat s;

	while((dirp = readdir(dir)))
	{
		if(!stat(dirp->d_name, &s))
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

inline
bool getargs(std::istream&) { return true; }

template<typename... Args> inline
bool getargs(std::istream& is, str& t, Args&... args);

template<typename T, typename... Args> inline
bool getargs(std::istream& is, T& t, Args&... args)
{
	if(is >> t)
		return getargs(is, args...);
	return false;
}

template<typename... Args> inline
bool getargs(std::istream& is, str& t, Args&... args)
{
	if(ios::getstring(is, t))
		return getargs(is, args...);
	return false;
}

template<typename... Args> inline
bool getargs(const str& s, Args&... args)
{
	std::istringstream iss(s);
	return getargs(iss, args...);
}

}} // sookee::ios

namespace soo { using namespace sookee::ios; }

#endif // _LIBSOOKEE_IOS_H_
