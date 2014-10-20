#ifndef LIBSOOKEE_IOS_H_
#define LIBSOOKEE_IOS_H_
/*
 *  Created on: 28 Jan 2012
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
#include <sookee/types/str_vec.h>
#include <sookee/log.h>

#include <unistd.h>
#include <ext/stdio_filebuf.h>
#include <wordexp.h>

#include <istream>
#include <ostream>
#include <limits>
#include <cstring>

namespace sookee { namespace ios {

using namespace sookee::log;
using namespace sookee::types;

std::istream& getstring(std::istream& is, str& s);
inline
std::istream& getstring(std::istream&& is, str& s) { return getstring(is, s); }

str expand_env(const str& var, int flags = 0);

/**
 * Get directory listing.
 * @return errno
 */
int ls(const str& folder, str_vec &files);

/**
 * Get directory listing.
 * @return errno
 */
bool ls(const str& folder, str_vec& folders, str_vec &files);

enum class ftype
{
	unk, reg, dir, chr, blk, fifo, lnk, sock, all
};

str_vec ls(const str& folder, ftype t = ftype::reg);

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

/**
 * Like std::ostream_iterator<> but doesn't
 * leave a spare separator at the end.
 */
template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
class ostream_serializer
: public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::basic_ostream<CharT>& os;
	std::basic_string<CharT> sep;
	std::basic_string<CharT> worksep;

public:
	ostream_serializer(std::basic_ostream<CharT>& os
		, const std::basic_string<CharT>& sep = " "): os(os), sep(sep) {}

	ostream_serializer& operator=(const T& value)
    {
		os << worksep << value;
		worksep = sep;
		return *this;
    }

	ostream_serializer& operator*() { return *this; }
	ostream_serializer& operator++() { return *this; }
	ostream_serializer& operator++(int) { return *this; }
};

inline
std::istream& sgl(std::istream& is, str& s, char d = '\n')
{
	return std::getline(is, s, d);
}

inline
std::istream& sgl(std::istream&& is, str& s, char d = '\n')
{
	return sgl(is, s, d);
}

inline
std::istream& ign(std::istream& is, char c = '\n')
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

inline
std::istream& ign(std::istream&& is, char c = '\n')
{
	return ign(is, c);
}

inline
std::istream& user_skip_line(std::istream& is)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename Type>
std::istream& user_get(std::istream& is, Type& t)
{
	if(user_skip_line(is >> t))
		return is;

	// Fix the stream
	is.clear();
	user_skip_line(is).setstate(std::ios::failbit);

	return is;
}

template<typename Type>
std::istream& user_insist(std::istream& is, std::ostream& os, Type& t
	, const str& question, const str& error, siz retries)
{
	os << question;

	while(retries-- && !ios::user_get<int>(is, t))
	{
		is.clear();
		os << error << '\n' << question;
	}

	if(!retries)
		is.setstate(std::ios::failbit);

	return is;
}

// Allow for in >> ',' // parse literal comma
inline std::istream& operator>>(std::istream& is, const char& c)
{
	char i;
	if(is >> i && i != c)
		is.setstate(std::ios::failbit);
	return is;
}

// Allow for in << "  ,  " parse literal string
inline std::istream& operator>>(std::istream& is, const char* s)
{
	char c;
	if(is.setf(std::ios::fmtflags(0)) & std::ios::skipws)
	{
		is >> std::ws;
		while(std::isspace(*s))
			++s;
	}
	while(*s && is.get(c))
		if(*s++ != c)
			is.setstate(std::ios::failbit);
	return is;
}

inline std::istream& operator>>(std::istream& is, const std::string& s)
{
	char c;
	std::string::const_iterator i = s.begin();
	std::string::const_iterator e = s.end();
	if(is.setf(std::ios::fmtflags(0)) & std::ios::skipws)
	{
		is >> std::ws;
		while(std::isspace(*i))
			++i;
	}
	while(i != e && is.get(c))
		if(*i++ != c)
			is.setstate(std::ios::failbit);
	return is;
}

//using namespace __gnu_cxx;

typedef __gnu_cxx::stdio_filebuf<char> stdiobuf;
typedef __gnu_cxx::stdio_filebuf<wchar_t> wstdiobuf;

template<typename Char>
class basic_stdiostream
: public std::basic_iostream<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_iostream<char_type> stream_type;
	typedef __gnu_cxx::stdio_filebuf<char_type> buf_type;

protected:
	buf_type buf;

public:
	basic_stdiostream(int fd, const std::ios::openmode& mode)
	: stream_type(&buf)
	, buf(fd, mode)
	{
	}

	void close()
	{
		::close(buf.fd()); // interrupt blocking?
	}
};

typedef basic_stdiostream<char> stdiostream;
typedef basic_stdiostream<wchar_t> wstdiostream;
typedef std::shared_ptr<stdiostream> stdiostream_sptr;
typedef std::unique_ptr<stdiostream> stdiostream_uptr;

class Fork
{
	stdiostream_uptr stdip;
	stdiostream_uptr stdop;

	bool log_report(const str& msg, bool state = false)
	{
		log(msg);
		return state;
	}

	static std::iostream& get_null_iostream()
	{
		static stdiostream stdionull(-1, std::ios::in|std::ios::out);
		stdionull.setstate(std::ios::failbit|std::ios::badbit);
		return stdionull;
	}

public:

	std::istream& istream()
	{
		if(!stdip.get())
			return get_null_iostream();
		return *stdip;
	}

	std::ostream& ostream()
	{
		if(!stdop.get())
			return get_null_iostream();
		return *stdop;
	}

	bool exec(const str& dir, const str& prog)
	{
		pid_t pid;

		int pipe_w[2]; // write to child
		int pipe_r[2]; // read from child

		if(pipe(pipe_w) || pipe(pipe_r))
			return log_report(strerror(errno));

		if((pid = fork()) == -1)
			return log_report(strerror(errno));

		if(pid)
		{
			// parent
			stdip.reset(new(std::nothrow) stdiostream(pipe_r[0], std::ios::in));
			stdop.reset(new(std::nothrow) stdiostream(pipe_w[1], std::ios::out));

			close(pipe_r[1]);
			close(pipe_w[0]);

			if(!stdip.get() || !stdop.get())
				return log_report("Unable to create stdiostream object.");

			return true;
		}
		else
		{
			// child

//			rlimit lim;
//			if(!getrlimit(RLIMIT_NPROC, &lim))
//			{
//				lim.rlim_max += 10;
//				lim.rlim_cur += 10;
//				setrlimit(RLIMIT_NPROC, &lim);
//			}

			close(1);
			dup(pipe_r[1]); // lowest unused fd

			close(0);
			dup(pipe_w[0]); // lowest unused fd

			close(pipe_r[0]);
			close(pipe_r[1]);
			close(pipe_w[0]);
			close(pipe_w[1]);

			str prog_name = dir + "/" + prog;

			if(!dir.empty())
				chdir(dir.c_str());
			execlp(prog_name.c_str(), prog_name.c_str(), 0);
			log("ERROR: " << strerror(errno));
			return false; // execl() failed
		}

		return true;
	}
};

}} // sookee::ios

namespace soo { using namespace sookee::ios; }

#endif // LIBSOOKEE_IOS_H_
