#ifndef LIBSOOKEE_SSL_SOCKETSTREAM_H
#define LIBSOOKEE_SSL_SOCKETSTREAM_H

/*-----------------------------------------------------------------.
| Copyright (C) 2013 SooKee oasookee@gmail.com                     |
'------------------------------------------------------------------'

This code is was created from code (C) Copyright Nicolai M. Josuttis 2001
with the following Copyright Notice:
*/

/* The following code declares classes to read from and write to
 * file descriptore or file handles.
 *
 * See
 *      http://www.josuttis.com/cppcode
 * for details and the latest version.
 *
 * - open:
 *      - integrating BUFSIZ on some systems?
 *      - optimized reading of multiple characters
 *      - stream for reading AND writing
 *      - i18n
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * Version: Jul 28, 2002
 * History:
 *  Jul 28, 2002: bugfix memcpy() => memmove()
 *                fdinbuf::underflow(): cast for return statements
 *  Aug 05, 2001: first public version
 */

 /*

Permission to copy, use, modify, sell and distribute this software
is granted under the same conditions.

'----------------------------------------------------------------*/

#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sookee/log.h>
#include <sookee/types/str_vec.h>

namespace sookee { namespace net {

using namespace sookee::log;
using namespace sookee::types;

typedef std::string str;
typedef size_t siz;

struct ssl_connection
{
	int sock;
	SSL *ssl;
	SSL_CTX *ctx;
	str_vec errors;
	ssl_connection(): sock(-1), ssl(0), ctx(0) {}
// TODO:
//	~ssl_connection()
//	{
//		if(ssl)
//			SSL_free(ssl);
//		if(ctx)
//			SSL_CTX_free(ctx);
//	}
};

template<typename Char>
class basic_ssl_socketbuf
: public std::basic_streambuf<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_streambuf<char_type> buf_type;
	typedef std::basic_ostream<char_type> stream_type;
	typedef typename buf_type::int_type int_type;
	typedef typename std::basic_streambuf<Char>::traits_type traits_type;

protected:
	static const int char_size = sizeof(char_type);
	static const int SIZE = 128;

	ssl_connection& conn;
	std::vector<char_type> ibuf;
	std::vector<char_type> obuf;

public:
	basic_ssl_socketbuf(ssl_connection& conn)
	: conn(conn)
	, ibuf(SIZE)
	, obuf(SIZE)
	{
		buf_type::setp(obuf.data(), obuf.data() + obuf.size() - 1);
		buf_type::setg(ibuf.data(), ibuf.data(), ibuf.data());
	}

	virtual ~basic_ssl_socketbuf()
	{
		sync();
	}

//	void set_ssl_connection(const ssl_connection& conn)
//	{
//		bug_fun();
//		bug_var(conn.sock);
//		this->conn = conn;
//	}
//	ssl_connection get_ssl_connection() const { return this->conn; }

protected:

	int output_buffer()
	{
//		bug_fun();
//		bug_var(conn.ssl);
		int num = buf_type::pptr() - buf_type::pbase();
//		bug_var((void*)buf_type::pptr());
//		bug_var((void*)buf_type::pbase());
//		bug_var(num);
		int ret;
		if((ret = SSL_write(conn.ssl, reinterpret_cast<char*>(obuf.data()), num * char_size)) != num)
		{
			conn.errors.emplace_back(ERR_error_string(SSL_get_error(conn.ssl, ret), nullptr));
			return traits_type::eof();
		}
		buf_type::pbump(-num);
		return num;
	}

	virtual int_type overflow(int_type c)
	{
		if(c != traits_type::eof())
		{
			*buf_type::pptr() = c;
			buf_type::pbump(1);
		}

		if(output_buffer() == traits_type::eof())
			return traits_type::eof();
		return c;
	}

	virtual int sync()
	{
		if(conn.ssl && output_buffer() == traits_type::eof())
			return traits_type::eof();
		return 0;
	}

	virtual int_type underflow()
	{
		if(buf_type::gptr() < buf_type::egptr())
			return *buf_type::gptr();

		int num;
		if((num = SSL_read(conn.ssl, reinterpret_cast<char*>(ibuf.data()), ibuf.size() * char_size)) <= 0)
		{
			conn.errors.emplace_back(ERR_error_string(SSL_get_error(conn.ssl, num), nullptr));
			return traits_type::eof();
		}

		buf_type::setg(ibuf.data(), ibuf.data(), ibuf.data() + num);
		return *buf_type::gptr();
	}
};

typedef basic_ssl_socketbuf<char> ssl_socketbuf;
typedef basic_ssl_socketbuf<wchar_t> ssl_wsocketbuf;

template<typename Char>
class basic_ssl_socketstream
: public std::basic_iostream<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_iostream<char_type> stream_type;
	typedef basic_ssl_socketbuf<char_type> buf_type;

protected:
	buf_type buf;
	ssl_connection conn;

public:
	basic_ssl_socketstream(): stream_type(&buf), buf(conn)
	{
		SSL_load_error_strings();
		SSL_library_init();
	}

	virtual ~basic_ssl_socketstream()
	{
		this->close();
	}

	str get_error() const
	{
		if(conn.errors.empty())
			return {};
		return conn.errors.back();
	}

	str_vec get_errors() const { return conn.errors; }

	void close()
	{
//		bug_fun();
//		bug_var(conn.sock);
		ssl_disconnect();
		stream_type::clear();
	}

	bool open(const std::string& host, uint16_t port = 443)
	{
//		bug_fun();
		close();
		return ssl_connect(host, port, conn);
	}

	bool tcp_connect(const str& host, siz port, int& sd)
	{
		close();

		if((sd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			log(strerror(errno));
			stream_type::setstate(std::ios::failbit);
			return false;
		}

		hostent* he;
		if(!(he = gethostbyname(host.c_str())))
		{
			::close(sd);
			stream_type::setstate(std::ios::failbit);
			return false;
		}

		sockaddr_in sin;
		std::copy(reinterpret_cast<char*>(he->h_addr)
			, reinterpret_cast<char*>(he->h_addr) + he->h_length
			, reinterpret_cast<char*>(&sin.sin_addr.s_addr));

		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr = *((in_addr*) he->h_addr);

		if(connect(sd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) < 0)
		{
			::close(sd);
			stream_type::setstate(std::ios::failbit);
			return false;
		}

		return true;
	}

//	void print_ERRs()
//	{
//		siz code;
//		while((code = ERR_get_error()) != 0)
//			std::cerr << ERR_error_string(code, 0) << '\n';
//	}
//
	void tcp_close(int& fd)
	{
		if(fd != -1)
		{
			::close(fd);
			fd = -1;
		}
	}

	bool ssl_connect(const str& host, siz port, ssl_connection& conn)
	{
		if(!tcp_connect(host, port, conn.sock))
		{
			stream_type::setstate(std::ios::badbit);
			return false;
		}

//		SSL_load_error_strings();
//		SSL_library_init();

		if(!(conn.ctx = SSL_CTX_new(SSLv23_client_method())))
		{
			stream_type::setstate(std::ios::badbit);
			tcp_close(conn.sock);
			return false;
		}
		if(!(conn.ssl = SSL_new(conn.ctx)))
		{
			SSL_CTX_free(conn.ctx);
			tcp_close(conn.sock);
			stream_type::setstate(std::ios::badbit);
			return false;
		}
		if(!SSL_set_fd(conn.ssl, conn.sock))
		{
			SSL_free(conn.ssl);
			SSL_CTX_free(conn.ctx);
			tcp_close(conn.sock);
			stream_type::setstate(std::ios::badbit);
			return false;
		}
		if(SSL_connect(conn.ssl) != 1)
		{
			SSL_free(conn.ssl);
			SSL_CTX_free(conn.ctx);
			tcp_close(conn.sock);
			stream_type::setstate(std::ios::badbit);
			return false;
		}

		return true;
	}

	void ssl_disconnect()
	{
		if(conn.sock != -1)
			::close(conn.sock);
		if(conn.ssl)
		{
			SSL_shutdown(conn.ssl);
			SSL_free(conn.ssl);
		}
		if(conn.ctx)
			SSL_CTX_free(conn.ctx);
	}

};

typedef basic_ssl_socketstream<char> ssl_socketstream;
typedef basic_ssl_socketstream<wchar_t> ssl_wsocketstream;

}} // sookee::net

namespace soo { using namespace sookee::net; }

#endif // LIBSOOKEE_SSL_SOCKETSTREAM_H
