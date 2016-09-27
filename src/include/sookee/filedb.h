#ifndef LIBSOOKEE_FILEDB_H
#define LIBSOOKEE_FILEDB_H
/*
 *  Created on: 07 Dec 2014
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oasookee@gmail.com                     |
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

#include <sookee/types/basic.h>
#include <sookee/types/stream.h>
#include <sookee/types/str_vec.h>
#include <sookee/log.h>

#include <unistd.h>
#include <sys/stat.h>

#include <istream>
#include <ostream>
#include <limits>
#include <cstring>

namespace sookee { namespace filedb {

// write binary

template<typename Type>
std::ostream& write(std::ostream& os, const Type& t)
{
	return os.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

inline
std::ostream& write(std::ostream& os, const std::string& s)
{
	write(os, s.size());
	return os.write(s.data(), s.size());
}

inline
std::ostream& write(std::ostream& os, const char* s)
{
	return os.write(s, std::strlen(s) + 1);
}

template<typename Arg, typename... Args>
std::ostream& write(std::ostream& os, const Arg& arg, const Args&... args)
{
	return write(write(os, arg), args...);
}

// read binary

template<typename Type>
std::istream& read(std::istream& is, Type& t)
{
	return is.read(reinterpret_cast<char*>(&t), sizeof(t));
}

inline
std::istream& read(std::istream& is, std::string& s)
{
	std::string::size_type len;
	read(is, len);
	s.resize(len);
	return is.read(&s[0], s.size());
}

inline
std::istream& read(std::istream& is, char* s)
{
	for(char c; is.get(c) && c; ++s)
		*s = c;
	*s = 0;
	return is;
}

template<typename Arg, typename... Args>
std::istream& read(std::istream& is, Arg& arg, Args&... args)
{
	return read(read(is, arg), args...);
}

// higher level

//template<typename PodType>
//struct FileDescriptor
//{
//	std::fstream fs;
//	FileDescriptor(const std::string& filename, std::ios::openmode mode)
//	: fs(filename, mode) {}
//};
//
//class FileDb
//{
//	struct header
//	{
//
//	};
//
//public:
//
//	template<typename PodType>
//	FileDescriptor<PodType> create(const std::string& filename)
//	{
//		return FileDescriptor<PodType>(filename, std::ios::in|std::ios::out);
//	}
//
//	template<typename PodType>
//	bool write(const std::string& filename, const PodType& data, std::streampos pos)
//	{
//		std::fstream fs(filename, std::ios::in|std::ios::out);
//
//		if(!fs)
//		{
//			log("E: opening file: " << filename);
//			return false;
//		}
//
//
//	}
//};

}} // sookee::filedb

namespace soo { using namespace sookee::filedb; }

#endif // LIBSOOKEE_FILEDB_H
