#ifndef LIBSOOKEE_IOS_H
#define LIBSOOKEE_IOS_H
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

#include <vector>

#include <sookee/types/basic.h>
#include <sookee/types/stream.h>
#include <sookee/types/str_vec.h>
#include <sookee/log.h>
#include <sookee/str.h>

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
#include <array>

namespace sookee { namespace ios {

using namespace sookee::log;
using namespace sookee::types;
using namespace sookee::utils;

std::istream& getstring(std::istream& is, str& s);
inline
std::istream& getstring(std::istream&& is, str& s) { return getstring(is, s); }

std::istream& getnested(std::istream& is, str& s, char d1 = '{', char d2 = '}');
inline
std::istream& getnested(std::istream&& is, str& s, char d1 = '{', char d2 = '}')
{
	return getnested(is, s, d1, d2);
}


str wordexp(str var, int flags = 0);
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

str_vec tree(const str& root);

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

	while(retries-- && !ios::user_get<Type>(is, t))
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

// secure user input with std::strings
inline
std::istream& getline(std::istream& is, std::string& s, std::streamsize num, char delim = '\n')
{
	std::unique_ptr<char[]> buf(new char[num + 1]);
	if(is.getline(buf.get(), num + 1, delim))
		s.assign(buf.get(), is.gcount() - 1);
	return is;
}

template<std::size_t MaxSize>
std::istream& get_buffer(std::istream& is, std::string& s, char delim = '\n')
{
	char buf[MaxSize + 1];
	if(is.getline(buf, sizeof(buf), delim))
		s.assign(buf, is.gcount() - 1);
	return is;
}

// retain type info for std::stringstreams

template<typename Type>
std::stringstream& operator<<(std::stringstream& ss, const Type& type)
{
	static_cast<std::ostream&>(ss) << type;
	return ss;
}

template<typename Type>
std::stringstream& operator<<(std::stringstream&& ss, const Type& type)
{
	static_cast<std::ostream&>(ss) << type;
	return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream& ss, Type& type)
{
	static_cast<std::istream&>(ss) >> type;
	return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream&& ss, Type& type)
{
	static_cast<std::istream&>(ss) >> type;
	return ss;
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
	: stream_type(/*&buf*/), buf(fd, mode)
	{
		this->rdbuf(&buf);
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
				log("E: Unable to kill process: " << pid);
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
			log("E: Unable to kill process: " << pid);
			return;
		}

		while(st_clk::now() < die_by)
		{
			if((stat(proc.c_str() , &s) == -1) && errno == ENOENT)
				return; // killed
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		log("E: Unable to kill process: " << pid);
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

	/**
	 *
	 * @param dir Spaced need to be escaped "just\ like\ \ this.txt"
	 * @param prog
	 * @param args
	 * @return
	 */
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

		// escaped because wordexp breaks at spaces
		//replace(dir, " ", "\\ ");
		//replace(prog, " ", "\\ ");

		//log("BEFORE");
		dir = wordexp(dir, WRDE_SHOWERR);
		//log("AFTER");
		prog = wordexp(prog, WRDE_SHOWERR);

		if(!dir.empty())
			if(chdir(dir.c_str()) == -1)
				log("E: [" << errno << "] " << strerror(errno) << ": " << dir);

		execlp(prog.c_str(), prog.c_str(), convert(args).c_str()..., (char*)0);

		log("E: [" << errno << "] " << strerror(errno));
		return false; // execl() failed
	}
};

// Stream Iterators/Wrappers

enum flags {none = 0, rebegin = 1, skip_blanks = 2};

class istream_line_iterator
{
	std::istream* is = nullptr;
	str line;
	bool skip = false;

public:
	istream_line_iterator(std::istream& is, int flags = none)
	: istream_line_iterator(&is, flags) {}
	istream_line_iterator(std::istream* is = nullptr, int flags = none)
	: is(is), skip(flags & skip_blanks)
	{
		if(is && (flags & rebegin))
		{
			is->clear();
			is->seekg(0);
		}
		++(*this);
	}

	istream_line_iterator& operator++()
	{
		if(is)
		{
			while(std::getline(*is, line) && trim_copy(line).empty()) {}
			if(!(*is))
				is = nullptr;
		}
		return *this;
	}

	istream_line_iterator operator++(int) { istream_line_iterator i(*this); return ++i; }

	const str& operator*() const { return line; }

	bool operator==(const istream_line_iterator& i) const
	{
		if(is != i.is)
			return false;

		if(is && is->tellg() != i.is->tellg())
			return false;

		return false;
	}
	bool operator!=(const istream_line_iterator& i) const { return is != i.is; }
};

template<typename DataType>
class value_iterable_istream
{
	std::istream& is;

public:
	value_iterable_istream(std::istream& is): is(is) {}

	std::istream_iterator<DataType> begin() { return std::istream_iterator<DataType>(is); }
	std::istream_iterator<DataType> end() { return std::istream_iterator<DataType>(); }
};

class line_iterable_istream
{
	std::istream& is;
	int flags;

public:
	line_iterable_istream(std::istream& is, int flags = none): is(is), flags(flags)
	{
		if(flags & rebegin)
		{
			is.clear();
			is.seekg(0);
		}
	}

	istream_line_iterator begin() { return istream_line_iterator(is, flags); }
	istream_line_iterator end() { return istream_line_iterator(); }
};


// TODO: add these ??
template<std::size_t SIZE>
std::istream& getline_s(std::istream& is, char(&buf)[SIZE], char delim = '\n')
{
	return is.getline(buf, SIZE, delim);
}

inline
std::istream& getline_s(std::istream& is, char* buf, std::size_t size, char delim = '\n')
{
	return is.getline(buf, size, delim);
}

inline
std::istream& getline_s(std::istream& is, std::string& buf, char delim = '\n')
{
	return getline_s(is, &buf[0], buf.size(), delim);
}

/**
 * This function requires std::ios::sync_with_stdio(false);
 * and may be implementation specific.
 *
 * @param s out parameter
 * @param wait time to wait before returning with no input
 * @return false if timed out, true if input received in time allotted
 */
inline
bool timed_input(std::string& s, std::chrono::milliseconds wait)
{
	using namespace std::chrono;

	steady_clock::time_point timeup = steady_clock::now() + wait;

	std::streamsize n;
	while(!(n = std::cin.rdbuf()->in_avail()) && steady_clock::now() < timeup)
		std::this_thread::sleep_for(milliseconds(10));

	if(!n)
		return false;

	s.resize(n);
	std::cin.read(&s[0], n);

	return true;
}

// untested
template<typename CharType, typename TraitsType = std::char_traits<CharType>>
class ibufferstream
: std::basic_istream<CharType, TraitsType>
{
public:
	using size_type = typename std::vector<CharType>::size_type;
	using string_type = std::basic_string<CharType, TraitsType>;
	using istream_type = std::basic_istream<CharType, TraitsType>;

private:
//	std::vector<char> buf;
	string_type const* buf;
	size_type gptr = 0;
	size_type pptr = 0;

	bool eofbit = false;
	bool failbit = false;

	void checkeof()
	{
		if(gptr == buf->size())
			eofbit = true;
	}

	void skipws()
	{
		while(gptr < buf->size() && buf[gptr] == ' ') {}
		checkeof();
	}

public:
	ibufferstream(string_type const& buf): istream_type(nullptr), buf(&buf) {}
	ibufferstream(ibufferstream&& ibs): istream_type(nullptr), buf(ibs.buf) { ibs.buf = nullptr; }
	ibufferstream(ibufferstream const&) = delete;

	ibufferstream& operator=(ibufferstream&& ibs)
	{
		buf = ibs.buf;
		ibs.buf = nullptr;
	}

	ibufferstream& operator=(ibufferstream const&) = delete;

	operator bool() const
	{
		return failbit;
	}

	ibufferstream& operator>>(char& c)
	{
//		bug_fun();
		skipws();
		if(!(gptr < buf->size()))
			failbit = true;
		else
		{
			c = buf[gptr++];
			checkeof();
		}
		return *this;
	}

	ibufferstream& operator>>(std::string& s)
	{
//		bug_fun();
		skipws();
		if(!(gptr < buf->size()))
			failbit = true;
		else
		{
			s.clear();
			while(gptr < buf->size() && buf[gptr] != ' ')
				s += buf[gptr++];
			checkeof();
		}
		return *this;
	}

	ibufferstream& operator>>(long long int& i)
	{
//		bug_fun();
		skipws();
		if(!(gptr < buf->size()))
			failbit = true;
		else
		{
			char* e;
			i = std::strtoll(buf->data() + gptr, &e, 10);
			if(e == buf->data())
				failbit = true;
			checkeof();
		}
		return *this;
	}

	ibufferstream& operator>>(long int& i)
	{
//		bug_fun();
		long long int lli;
		if((*this) >> lli)
		{
			if(lli > std::numeric_limits<long int>::max())
				failbit = true;
			i = lli;
		}

		return *this;
	}

	ibufferstream& operator>>(int& i)
	{
//		bug_fun();
		long long int lli;
		if((*this) >> lli)
		{
			if(lli > std::numeric_limits<int>::max())
				failbit = true;
			i = lli;
		}

		return *this;
	}

	ibufferstream& operator>>(unsigned long long int& i)
	{
//		bug_fun();
		skipws();
		if(!(gptr < buf->size()))
			failbit = true;
		else
		{
			char* e;
			i = std::strtoull(buf->data() + gptr, &e, 10);
			if(e == buf->data())
				failbit = true;
			checkeof();
		}
		return *this;
	}

	ibufferstream& operator>>(unsigned long int& i)
	{
//		bug_fun();
		unsigned long long int lli;
		if((*this) >> lli)
		{
			if(lli > std::numeric_limits<unsigned long int>::max())
				failbit = true;
			i = lli;
		}

		return *this;
	}

	ibufferstream& operator>>(unsigned int& i)
	{
//		bug_fun();
		unsigned long long int lli;
		if((*this) >> lli)
		{
			if(lli > std::numeric_limits<unsigned int>::max())
				failbit = true;
			i = lli;
		}

		return *this;
	}

	ibufferstream& operator>>(long double& d)
	{
//		bug_fun();
		skipws();
		if(!(gptr < buf->size()))
			failbit = true;
		else
		{
			char* e;
			d = std::strtold(buf->data() + gptr, &e);
			if(e == buf->data())
				failbit = true;
			checkeof();
		}
		return *this;
	}

	ibufferstream& operator>>(double& d)
	{
//		bug_fun();
		long double ld;
		if((*this) >> ld)
		{
			if(ld > std::numeric_limits<double>::max())
				failbit = true;
			d = ld;
		}
		return *this;
	}

	ibufferstream& operator>>(float& f)
	{
//		bug_fun();
		long double ld;
		if((*this) >> ld)
		{
			if(ld > std::numeric_limits<float>::max())
				failbit = true;
			f = ld;
		}
		return *this;
	}
};


}} // sookee::ios

namespace soo { using namespace sookee::ios; }

#endif // LIBSOOKEE_IOS_H
