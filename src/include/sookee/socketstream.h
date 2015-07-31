#ifndef LIBSOOKEE_SOCKETSTREAM_H
#define LIBSOOKEE_SOCKETSTREAM_H

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com                     |
'------------------------------------------------------------------'

// This is no longer true
//This code is was created from code (C) Copyright Nicolai M. Josuttis 2001
//with the following Copyright Notice:
//*/
//
///* The following code declares classes to read from and write to
// * file descriptore or file handles.
// *
// * See
// *      http://www.josuttis.com/cppcode
// * for details and the latest version.
// *
// * - open:
// *      - integrating BUFSIZ on some systems?
// *      - optimized reading of multiple characters
// *      - stream for reading AND writing
// *      - i18n
// *
// * (C) Copyright Nicolai M. Josuttis 2001.
// * Permission to copy, use, modify, sell and distribute this software
// * is granted provided this copyright notice appears in all copies.
// * This software is provided "as is" without express or implied
// * warranty, and with no claim as to its suitability for any purpose.
// *
// * Version: Jul 28, 2002
// * History:
// *  Jul 28, 2002: bugfix memcpy() => memmove()
// *                fdinbuf::underflow(): cast for return statements
// *  Aug 05, 2001: first public version
// */

 /*

Permission to copy, use, modify, sell and distribute this software
is granted under the same conditions.

'----------------------------------------------------------------*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <streambuf>
#include <istream>
#include <ostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring> // strerror()
#include <cerrno>

#include <sookee/types/basic.h>
#include <sookee/bug.h>

#ifdef __clang__
#	include "stdio_filebuf.h"
#else
#	include <ext/stdio_filebuf.h>
#endif

namespace sookee { namespace net {

using namespace __gnu_cxx;
using namespace sookee::bug;
using namespace sookee::types;

template<typename Char>
class basic_netstream
: public std::basic_iostream<Char>
{
	const std::ios::openmode mode = std::ios::in|std::ios::out;

public:
	basic_netstream(basic_netstream&& ns): std::basic_iostream<Char>(ns.rdbuf(nullptr))
	{
	}

	basic_netstream()
	: std::basic_iostream<Char>(nullptr)
	{
	}

	basic_netstream(int sd)
	: std::basic_iostream<Char>(new stdio_filebuf<Char>(sd, mode))
	{
	}

	basic_netstream(const str& host, const in_port_t port)
	: std::basic_iostream<Char>(nullptr)
	{
		open(host, port, mode);
	}

	virtual ~basic_netstream()
	{
		close();
	}

	void open_old(const str& host, const in_port_t port)
	{
		hostent* he;
		if(!(he = gethostbyname(host.c_str())))
		{
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			return;
		}

		int sd;
		if((sd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			return;
		}

		sockaddr_in sin;
		memcpy(&sin.sin_addr.s_addr, he->h_addr, he->h_length);

		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr = *((in_addr*) he->h_addr);

		if(connect(sd, (sockaddr*) &sin, sizeof(sin)) < 0)
		{
			::close(sd);
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			return;
		}

		delete std::basic_iostream<Char>::rdbuf(new (std::nothrow) stdio_filebuf<Char>(sd, mode));

		if(!std::basic_iostream<Char>::rdbuf())
			std::basic_iostream<Char>::setstate(std::ios::failbit); // memory fail
	}

	// TODO: decide how to handle err_message
	str error;

	bool open(const str& host, long port)
	{
		return open(host, std::to_string(port));
	}

	bool open(const str& host, const str& port)
	{
		addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
		hints.ai_socktype = SOCK_STREAM;

		addrinfo* res;
		if(int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
		{
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			error = gai_strerror(status);
			return false;
		}

		// try to connect to each
		int sd;
		addrinfo* p;
		for(p = res; p; p = p->ai_next)
		{
			if((sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
				continue;
			if(!connect(sd, p->ai_addr, p->ai_addrlen))
				break;
			::close(sd);
		}

		freeaddrinfo(res);

		if(!p)
		{
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			error = "unable to find connection";
			return false;
		}

		delete std::basic_iostream<Char>::rdbuf(new (std::nothrow) stdio_filebuf<Char>(sd, mode));

		if(!std::basic_iostream<Char>::rdbuf())
			std::basic_iostream<Char>::setstate(std::ios::failbit); // memory fail
		return true;
	}

	void close()
	{
		delete std::basic_iostream<Char>::rdbuf(nullptr);
	}
};

typedef basic_netstream<char> netstream;
typedef basic_netstream<wchar_t> wnetstream;

//template<typename Char>
//class basic_socketbuf
//: public std::basic_streambuf<Char>
//{
//public:
//	typedef Char char_type;
//	typedef std::basic_streambuf<char_type> buf_type;
//	typedef std::basic_ostream<char_type> stream_type;
//	typedef typename buf_type::int_type int_type;
//	typedef typename std::basic_streambuf<Char>::traits_type traits_type;
//
//protected:
//
//	static const int char_size = sizeof(char_type);
//	static const int SIZE = 1024;
//
//	int sock;
//	char_type* ibuf;
//	char_type* obuf;
//
//public:
//	basic_socketbuf()
//	: sock(-1)
//	, ibuf(new char_type[SIZE])
//	, obuf(new char_type[SIZE])
//	{
//		buf_type::setp(obuf, obuf + (SIZE - 1));
//		buf_type::setg(ibuf, ibuf, ibuf);
//	}
//
//	basic_socketbuf(basic_socketbuf&& sb)
//	: sock(sb.sock)
//	, ibuf(sb.ibuf)
//	, obuf(sb.obuf)
//	{
//
//		buf_type::setg(sb.eback(), sb.gptr(), sb.egptr());
//		buf_type::setp(sb.pbase(), sb.epptr());
//		buf_type::pbump(sb.pptr() - sb.pbase());
//
//		sb.sock = -1;
//		sb.ibuf = new char_type[SIZE];
//		sb.obuf = new char_type[SIZE];
//		sb.setp(sb.obuf, sb.obuf + (SIZE - 1));
//		sb.setg(sb.ibuf, sb.ibuf, sb.ibuf);
//	}
//
//	virtual ~basic_socketbuf()
//	{
//		sync();
//		delete[] ibuf;
//		delete[] obuf;
//	}
//
//	void set_socket(int sock) { this->sock = sock; }
//	int get_socket() { return this->sock; }
//
//protected:
//
//	int output_buffer()
//	{
//		int num = (int)(buf_type::pptr() - buf_type::pbase());
//		if(send(sock, reinterpret_cast<char*>(obuf), num * char_size, 0) != num)
//			return traits_type::eof();
//		buf_type::pbump(-num);
//		return num;
//	}
//
//	virtual int_type overflow(int_type c)
//	{
//		if(c != traits_type::eof())
//		{
//			*buf_type::pptr() = (char_type)c;
//			buf_type::pbump(1);
//		}
//
//		if(output_buffer() == traits_type::eof())
//			return traits_type::eof();
//		return c;
//	}
//
//	virtual int sync()
//	{
//		if(output_buffer() == traits_type::eof())
//			return traits_type::eof();
//		return 0;
//	}
//
//	virtual int_type underflow()
//	{
//		if(buf_type::gptr() < buf_type::egptr())
//			return *buf_type::gptr();
//
//		int num;
//		if((num = recv(sock, reinterpret_cast<char*>(ibuf), SIZE * char_size, 0)) <= 0)
//			return traits_type::eof();
//
//		buf_type::setg(ibuf, ibuf, ibuf + num);
//		return *buf_type::gptr();
//	}
//};
//
//typedef basic_socketbuf<char> socketbuf;
//typedef basic_socketbuf<wchar_t> wsocketbuf;
//
//template<typename Char>
//class basic_socketstream
//: public std::basic_iostream<Char>
//{
//public:
//	typedef Char char_type;
//	typedef std::basic_iostream<char_type> stream_type;
//	typedef basic_socketbuf<char_type> buf_type;
//
//protected:
//	buf_type buf;
//
//public:
//	basic_socketstream(): stream_type(&buf) {}
//	basic_socketstream(int s): stream_type(&buf) { buf.set_socket(s); }
//	basic_socketstream(basic_socketstream&& ss)
//	: buf(std::move(ss.buf))
//	{
//	}
//
//	virtual ~basic_socketstream()
//	{
//		close();
//	}
//
//	void close()
//	{
////		bug_func();
////		bug_var(buf.get_socket());
//		if(buf.get_socket() != -1)
//			::close(buf.get_socket());
//		buf.set_socket(-1);
//		stream_type::clear();
//	}
//
//	operator bool() const { return static_cast<bool>(*this); }//*((std::basic_iostream<Char>*)this); }
//
//	bool open(const std::string& host, in_port_t port, int type = SOCK_STREAM, bool nb = false)
//	{
//		close();
//
//		int sd;
//		hostent* he;
//
//		if((sd = socket(PF_INET, type, 0)) == -1)
//		{
////			log(strerror(errno));
//			stream_type::setstate(std::ios::failbit);
//			return false;
//		}
//
//		if(!(he = gethostbyname(host.c_str())))
//		{
//			::close(sd);
//			stream_type::setstate(std::ios::failbit);
//			return false;
//		}
//
//		sockaddr_in sin;
//		std::copy(reinterpret_cast<char*>(he->h_addr)
//			, reinterpret_cast<char*>(he->h_addr) + he->h_length
//			, reinterpret_cast<char*>(&sin.sin_addr.s_addr));
//
//		sin.sin_family = AF_INET;
//		sin.sin_port = htons(port);
//		sin.sin_addr = *((in_addr*) he->h_addr);
//
//		if(connect(sd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) < 0)
//		{
//			::close(sd);
//			stream_type::setstate(std::ios::failbit);
//			return false;
//		}
//
//		if(nb)
//			fcntl(sd, F_SETFL, O_NONBLOCK);
//		buf.set_socket(sd);
//
//		return (bool)*this;
//	}
//};
//
//typedef basic_socketstream<char> socketstream;
//typedef basic_socketstream<wchar_t> wsocketstream;

}} // sookee::net

namespace soo { using namespace sookee::net; }

#endif // LIBSOOKEE_SOCKETSTREAM_H
