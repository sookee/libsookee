#pragma once
#ifndef _LIBSOOKEE_NETWORK_H
#define _LIBSOOKEE_NETWORK_H

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

#include <sookee/types.h>

#include <map>
#include <string>
#include <ctime>

namespace sookee { namespace net {

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
typedef str_pair header_pair;
typedef header_pair header;
typedef header_map::iterator header_iter;
typedef header_map::const_iterator header_citer;


str urlencode(const str& url);

str get_cookie_header(const cookie_jar& cookies, const str& domain);

/**
 * Read the HTTP headers from std::istream is into the supplied
 * header_map.
 * If the returned headers do not have a HTTP 200 OK response code then
 * the stream is set to a fail state and the headers are not read.
 *
 * @param is The input stream to read the HTTP headres from.
 * @param headers The header_map intowhich the headers will be read
 * from a successful HTTP response.
 *
 * @return The passed in stread. If the headers do not contain a
 * successful response code (200 OK) then the stream is set to a
 * failed state.
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
str urldecode(std::string s);

str html_to_text(const str& html);
void html_to_text(std::istream& i, std::ostream& o);

bool hostname_to_ip(const str& hostname , str& ip);

}} // sookee::net

namespace soo { using namespace sookee::net; }

#endif // _LIBSOOKEE_NETWORK_H
