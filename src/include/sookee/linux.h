/*
 *  Created on: Mar 26, 2015
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_LINUX_H_
#define LIBSOOKEE_LINUX_H_

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
//#include <sookee/types/stream.h>
//#include <sookee/types/map.h>
//#include <sookee/types/str_vec.h>
//#include <sookee/ios.h>

#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <stdexcept>

namespace sookee { namespace linux {

//using namespace sookee;
//using namespace sookee::ios;
using namespace sookee::types;

/**
 * Return the current working directory as a string
 * @return
 */
str cwd()
{
	str s;
	char* dir = getcwd(0, 0);
	if(dir)
		s = dir;
	std::free(dir);
	return s;
}


/**
 * Sets a new working directory for the current scope.
 */
struct cwd_scoper
{
	str old_dir;
	cwd_scoper(const str& new_dir = "."): old_dir(cwd()) { chdir(new_dir.c_str()); }
	~cwd_scoper() { chdir(old_dir.c_str()); }
};

/**
 * Open a directory, using dirent's opendir(), and close it at
 * the end of the scope.
 */
struct dirent_scoper
{
	std::string name;
	DIR* dir = nullptr;
	dirent_scoper(const std::string& name = "."): name(name)
	{
		if(!(this->dir = opendir(name.c_str())))
			throw std::runtime_error(std::strerror(errno));
	}

	dirent_scoper(dirent_scoper&& dir)
	: name(std::move(name)), dir(dir.dir)
	{
		dir.dir = nullptr;
	}

	dirent_scoper(const dirent_scoper& dir)
	: name(dir.name)
	{
		if(!(this->dir = opendir(name.c_str())))
			throw std::runtime_error(std::strerror(errno));

		long pos;
		if((pos = telldir(dir.dir)) == -1)
			throw std::runtime_error(std::strerror(errno));
		seekdir(this->dir, pos);
	}

	~dirent_scoper() { if(dir) closedir(dir); }

	dirent_scoper& operator=(const dirent_scoper& dir)
	{
		this->name = dir.name;

		if(!(this->dir = opendir(name.c_str())))
			throw std::runtime_error(std::strerror(errno));

		long pos;
		if((pos = telldir(dir.dir)) == -1)
			throw std::runtime_error(std::strerror(errno));
		seekdir(this->dir, pos);

		return *this;
	}

	DIR* operator->() { return dir; }
};

/**
 * Provide iterator access to a file directory
 */
class file_directory
{
	std::string name;

public:
	file_directory(const std::string& name): name(name) {}

	struct iterator
	{
		std::string name; // needed for copy
		dirent_scoper dir;
		dirent* ent = nullptr; // null = end

		iterator() {}

		iterator(const std::string& name)
		: name(name), dir(name)
		{
			errno = 0;
			if(!(this->ent = readdir(dir.dir)) && errno)
				throw std::runtime_error(std::strerror(errno));
		}

		iterator(iterator&& i)
		: name(std::move(i.name)), dir(std::move(i.dir)), ent(i.ent)
		{
			i.ent = nullptr;
		}

		iterator(const iterator& i)
		: name(i.name), dir(name)
		{
			rewinddir(dir.dir);

			errno = 0;
			if(!(this->ent = readdir(dir.dir)) && errno)
				throw std::runtime_error(std::strerror(errno));

			while(this->ent && this->ent->d_ino != i.ent->d_ino)
				if(!(this->ent = readdir(dir.dir)) && errno)
					throw std::runtime_error(std::strerror(errno));
		}

		iterator& operator=(const iterator& i)
		{
			this->dir = i.dir;

			this->ent = nullptr;

			if(i.ent)
			{
				rewinddir(dir.dir);

				errno = 0;
				if(!(this->ent = readdir(dir.dir)) && errno)
					throw std::runtime_error(std::strerror(errno));

				while(this->ent && this->ent->d_ino != i.ent->d_ino)
					if(!(this->ent = readdir(dir.dir)) && errno)
						throw std::runtime_error(std::strerror(errno));
			}
			return *this;
		}

		bool operator==(const iterator& i) const { return ent == i.ent; }
		bool operator!=(const iterator& i) const { return !(*this == i); }

		iterator& operator++()
		{
			errno = 0;

			if(!(this->ent = readdir(dir.dir)) && errno)
				throw std::runtime_error(std::strerror(errno));

			return *this;
		}

		iterator operator++(int)
		{
			iterator i(*this);
			++(*this);
			return i;
		}

		std::string operator*() const { return ent->d_name ? ent->d_name : ""; }
	};

//	iterator begin() { return iterator(name); }
//	iterator end() { return iterator(); }

	// only const makes sense as we can't change
	// the file names using this.

	iterator begin() const { return iterator(name); }
	iterator end() const { return iterator(); }
};

}} // ::sookee::linux

namespace soo { using namespace sookee::linux; }

#endif // LIBSOOKEE_LINUX_H_
