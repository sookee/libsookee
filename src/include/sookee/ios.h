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
#include <sookee/types/stream.h>
#include <sookee/types/str_vec.h>
#include <sookee/log.h>

#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

#ifdef __clang__
#	include "stdio_filebuf.h"
#else
#	include <ext/stdio_filebuf.h>
#endif

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

std::istream& getnested(std::istream& is, str& s, char d1 = '{', char d2 = '}');
inline
std::istream& getnested(std::istream&& is, str& s, char d1 = '{', char d2 = '}')
{
	return getnested(is, s, d1, d2);
}


str wordexp(const str& var, int flags = 0);
// [[deprecated]]
inline str expand_env(const str& var, int flags = 0) { return wordexp(var, flags); }

// return vector
str_vec wordexp_vec(const str& var, int flags = 0);
// return error
int wordexp_vec(const str& var, str_vec& vars, int flags = 0);
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

// secure user input with sts::strings
inline
std::istream& getline(std::istream& is, std::string& s, std::streamsize num, char delim = '\n')
{
	s.resize(num);
	return is.getline(&s[0], num, delim);
}


//using namespace __gnu_cxx;

typedef __gnu_cxx ::stdio_filebuf<char> stdiobuf;
typedef __gnu_cxx ::stdio_filebuf<wchar_t> wstdiobuf;

template<typename Char>
class basic_stdio_stream: public std::basic_iostream<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_iostream<char_type> stream_type;
	typedef __gnu_cxx ::stdio_filebuf<char_type> buf_type;

protected:
	buf_type buf;

public:
	basic_stdio_stream(int fd, const std::ios::openmode& mode)
	:
	stream_type(&buf),
		buf(fd, mode)
	{
	}

	void close()
	{
		::close(buf.fd()); // interrupt blocking?
	}
};

typedef basic_stdio_stream<char> stdio_stream;
typedef basic_stdio_stream<wchar_t> wstdio_stream;

typedef std::shared_ptr<stdio_stream> stdio_stream_sptr;
typedef std::unique_ptr<stdio_stream> stdio_stream_uptr;

//typedef std::vector<stdio_stream_uptr> stdios_uptr_vec;

class Fork
{
	typedef std::vector<stdio_stream_uptr> stdios_uptr_vec;
	typedef std::vector<std::array<int, 2>> pipe_vec;

	typedef stdios_uptr_vec::size_type size_type;

	enum { R = 0, W = 1, I = 0, O = 1, E = 2 };

	pid_t pid = -1;

	stdios_uptr_vec stdp;

	char** env = nullptr;

	bool log_report(const str& msg, bool state = false)
	{
		log(msg);
		return state;
	}

	static stdio_stream& get_null_iostream()
	{
		static stdio_stream stdionull(-1, std::ios::in | std::ios::out);
		stdionull.setstate(std::ios::failbit | std::ios::badbit);
		return stdionull;
	}

	std::string convert(const std::string& s) { return s; }

	template<typename Type>
	std::string convert(Type t)
	{
		return static_cast<std::stringstream&>(std::stringstream() << t).str();
	}

public:
	struct pipe_desc
	{
		int fd = -1;
		uns io = I;
		pipe_desc(uns io, int fd): fd(fd), io(io) {}
	};
private:

	std::vector<pipe_desc> fds;

	template<typename Arg, typename... Args>
	void set_pipes_r(Arg arg, Args... args)
	{
		fds.push_back(arg);
		set_pipes_r(args...);
	}

	void set_pipes_r()
	{
//		for(auto&& fd: fds)
//			bug('{' << fd.fd << ", " << fd.io << '}');
	}

	static void killer(pid_t pid, st_duration wait)
	{
		st_time_point die_by = st_clk::now() + wait;

		if(::kill(pid, SIGTERM) == -1)
		{
			if(::kill(pid, SIGKILL) == -1)
			{
				log("ERROR: Unable to kill process: " << pid);
				return;
			}
		}

		struct stat s;
		str proc = "/proc/" + std::to_string(pid);

		while(st_clk::now() < die_by)
		{
			if((stat(proc.c_str() , &s) == -1) && errno == ENOENT)
				return; // killed
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		if(::kill(pid, SIGKILL) == -1)
		{
			log("ERROR: Unable to kill process: " << pid);
			return;
		}

		while(st_clk::now() < die_by)
		{
			if((stat(proc.c_str() , &s) == -1) && errno == ENOENT)
				return; // killed
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		log("ERROR: Unable to kill process: " << pid);
	}

public:

	/**
	 * Currently not working for > 3 file descriptore
	 * When a 4th is opened, 0 (stdin) fails and so does
	 * the 4th (3)
	 * @param fds
	 */
	Fork(): stdp(3) {}

	Fork(Fork&& fork) { (*this) = std::move(fork); }

	Fork& operator=(Fork&& fork)
	{
		if(&fork == this)
			return *this;

		this->kill();

		for(auto&& s: fork.stdp)
			stdp.emplace_back(s.release());

		env = fork.env;
		fork.env = nullptr;

		pid = fork.pid;
		fork.pid = -1;

		return *this;
	}

	void kill(st_duration wait = std::chrono::seconds(3))
	{
		for(auto&& s: stdp)
			s.release();
		stdp.clear();

		if(pid > 0)
			std::thread(killer, pid, wait).detach();
		pid = -1;
	}

	void setenv(char* env[]) { this->env = env; }

	explicit operator bool() const
	{
		for(auto& s: stdp)
			if(!s || !*s)
				return false;
		return pid != -1;
	}

	pid_t get_pid() const { return pid; }

	/**
	 * Get arbitrary file descriptor as stream
	 */
	stdio_stream& stdfd(int fd)
	{
		if(!(size_type(fd) < stdp.size()) || !stdp[fd])
			return get_null_iostream();
		return *stdp[fd];
	}

	/**
	 * Get named file descriptor 0 as stream
	 */
	stdio_stream& stdin() { return stdfd(I); }

	/**
	 * Get named file descriptor 1 as stream
	 */
	stdio_stream& stdout() { return stdfd(O); }

	/**
	 * Get named file descriptor 2 as stream
	 */
	stdio_stream& stderr() { return stdfd(E); }

	template<typename... Args>
	void set_pipes(Args... args)
	{
		fds.clear();
		set_pipes_r(args...);
	}

	template<typename... Args>
	bool exec(str dir, str prog, Args... args)
	{
		//bug_var(stdp.size());
		pipe_vec pipes(stdp.size());

		for(auto& a: pipes)
			if(pipe(a.data()))
				return log_report(strerror(errno));

		if(env)
			environ = env;

		if((pid = fork())) // parent
		{
			if(pid < 0)
				return log_report(std::strerror(errno));

			if(!stdp.empty())
			{
				stdp[0].reset(new (std::nothrow) stdio_stream(pipes[0][W], std::ios::out));
				close(pipes[0][R]);
			}

			for(unsigned p = 1; p < stdp.size(); ++p)
			{
				stdp[p].reset(new (std::nothrow) stdio_stream(pipes[p][R], std::ios::in));
				close(pipes[p][W]);
			}

			for(const auto& s: stdp)
				if(!s)
					return log_report("Unable to create stdiostream object.");

			return true;
		}

		// child

		for(int fd = pipes.size() - 1; fd > 0; --fd)
		{
			close(fd);
			dup(pipes[fd][W]);
		}

		if(!pipes.empty())
		{
			close(0);
			dup(pipes[0][R]);
		}

		for(auto& p: pipes)
			for(auto& fd: p)
				close(fd);

		dir = expand_env(dir, WRDE_SHOWERR);
		prog = expand_env(prog, WRDE_SHOWERR);

		if(!dir.empty())
			chdir(dir.c_str());

		execlp(prog.c_str(), prog.c_str(), convert(args).c_str()..., (char*)0);

		log("ERROR: [" << errno << "] " << strerror(errno));
		return false; // execl() failed
	}
};

}} // sookee::ios

namespace soo { using namespace sookee::ios; }

#endif // LIBSOOKEE_IOS_H_
