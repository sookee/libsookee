#ifndef LIBSOOKEE_NETWORK_H
#define LIBSOOKEE_NETWORK_H

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com                     |
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
#include <sookee/socketstream.h>
#include <sookee/ssl_socketstream.h>
#include <sookee/log.h>

#include <map>
#include <string>
#include <ctime>

namespace sookee { namespace net {

using namespace sookee::log;
using namespace sookee::types;

struct cookie
{
	str key;
	str val;
	str domain;
	str path;
	time_t expires = 0;
	bool secure;
	bool httponly;

	cookie();
	cookie(const str& data);
	bool is_expired();
};

typedef std::map<str, cookie> cookie_map;
typedef std::pair<const str, cookie> cookie_pair;
typedef cookie_map cookie_jar;
typedef str_mmap header_map;
typedef str_map_vt header_pair;
typedef header_pair header;
typedef header_map::iterator header_iter;
typedef header_map::const_iterator header_citer;


str urlencode(const str& url);

str get_cookie_header(const cookie_jar& cookies, const str& domain);

std::ostream& write_cookie_headers(std::ostream& os, const cookie_jar& cookies);

/**
 * Read the HTTP headers from std::istream is into the supplied
 * header_map.
 *
 * All header keys are made lower-case.
 *
 * @param is The input stream to read the HTTP headres from.
 * @param headers The header_map intowhich the headers will be read
 * from a successful HTTP response.
 *
 * @return The passed in stream.
 */
std::istream& read_http_headers(std::istream&is, header_map& headers);

std::istream& read_http_cookies(std::istream& is, const header_map& headers, cookie_jar& cookies);

/**
 * Read all the data chunks into a string. This function is useful when a
 * HTTP server returns data in chunk format indicated by the header field
 * "Transfer-Encoding: chunked".
 *
 * @param is A std::istream with get pointer at the start of the first chunk.
 * @param data The string to read all the chunks into.
 *
 * @return A reference to the std::istream object that was passed in as
 * a parameter.
 */
std::istream& read_chunked_encoding(std::istream&is, str& data);

std::istream& read_http_response_data(std::istream&is, const header_map& headers, str& data);

// HTML

std::istream& read_tag(std::istream& is, std::ostream& os, const str& tag);
std::istream& read_tag_by_att(std::istream& is, std::ostream& os, const str& tag, const str& att, const str& val);
/**
 * Assumes XML (XHTML)
 * If called on the start of a tag '<' reads whole tag, including sub-tags.
 * Otherwise reads up to the tag.
 */
std::istream& read_element(std::istream& is, str& tag);

str fix_entities(str s);
str urldecode(str s);

str html_to_text(const str& html);
void html_to_text(std::istream& i, std::ostream& o);

bool hostname_to_ip(const str& hostname , str& ip);

class http_stream
{
public:
	virtual ~http_stream() {}

	virtual bool open(const std::string& host, in_port_t port, bool nb = false) = 0;
	virtual bool is_open() const = 0;
	virtual void close() = 0;

	virtual str get(const str& path) = 0;
};

// TODO: these are buggy
class httpi_stream
: public http_stream
, public net::socketstream
{
	str host;

public:
//	using net::socketstream::socketstream;
	httpi_stream(): net::socketstream() {}
	httpi_stream(int s): net::socketstream() { buf.set_socket(s); }

	str user_agent = "none";
	str accept = "text/html";
	net::header_map headers;

	bool open(const std::string& host, in_port_t port, bool nb = false) override
	{
		this->host.clear();
		if(net::socketstream::open(host, port, SOCK_STREAM, nb))
			this->host = host;
		return is_open();
	}

	bool is_open() const override { return !host.empty(); }

	void close() override
	{
		host.clear();
		net::socketstream::close();
	}

	operator std::iostream&() { return *this; }

	str get(const str& path) override
	{
		net::socketstream& ss = *this;

		ss << "GET " << path << " HTTP/1.1\r\n";
		ss << "Host: " << host << "\r\n";
		ss << "User-Agent: " << user_agent << "\r\n";
		ss << "Accept: " << accept << "\r\n";
		ss << "\r\n" << std::flush;

		if(!net::read_http_headers(ss, headers))
		{
			log("ERROR reading headers.");
			return false;
		}

		str html;
		if(!net::read_http_response_data(ss, headers, html))
		{
			log("ERROR reading response data.");
			return false;
		}

		return html;
	}
};

class https_stream
: public http_stream
, public net::ssl_socketstream
{
	str host;

public:
	using net::ssl_socketstream::ssl_socketstream;

	str user_agent = "none";
	str accept = "text/html";
	net::header_map headers;

	operator std::iostream&() { return *this; }

	bool open(const std::string& host, in_port_t port, bool nb = false) override
	{
		(void) nb;
		this->host.clear();
		if(net::ssl_socketstream::open(host, port))//, SOCK_STREAM, nb))
			this->host = host;
		return is_open();
	}

	bool is_open() const override { return !host.empty(); }

	void close() override
	{
		host.clear();
		net::ssl_socketstream::close();
	}

	str get(const str& path) override
	{
		net::ssl_socketstream& ss = *this;

		ss << "GET " << path << " HTTP/1.1\r\n";
		ss << "Host: " << host << "\r\n";
		ss << "User-Agent: " << user_agent << "\r\n";
		ss << "Accept: " << accept << "\r\n";
		ss << "\r\n" << std::flush;

		if(!net::read_http_headers(ss, headers))
		{
			log("ERROR reading headers.");
			return false;
		}

		str html;
		if(!net::read_http_response_data(ss, headers, html))
		{
			log("ERROR reading response data.");
			return false;
		}

		return html;
	}
};

}} // sookee::net

namespace soo { using namespace sookee::net; }

#endif // LIBSOOKEE_NETWORK_H
