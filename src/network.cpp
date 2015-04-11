/*
 * network.cpp
 *
 *  Created on: 12 Aug 2011
 *      Author: oasookee@gmail.com
 */
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
#include <sookee/network.h>

#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <sookee/str.h>
#include <sookee/bug.h>
#include <sookee/log.h>

#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <netdb.h>	//hostent
#include <arpa/inet.h>
#include <netinet/in.h>

namespace sookee { namespace net {

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::types;
using namespace sookee::utils;

cookie::cookie()
: secure(false)
, httponly(false)
{
}

cookie::cookie(const str& data)
: secure(false)
, httponly(false)
{
//	bug_func();
	std::istringstream iss(data);
	std::getline(iss, key, '=');
	std::getline(iss, val, ';');
	iss >> std::ws;

//	bug_var(key);
//	bug_var(val);

	str att;
	while(std::getline(iss, att, '='))
	{
//		bug_var(att);
		if(att == "Domain")
			std::getline(iss, domain, ';');
		else if(att == "Path")
			std::getline(iss, path, ';');
		else if(att == "Expires")
		{
			std::getline(iss, att, ';');
			// Wdy, DD-Mon-YYYY HH:MM:SS GMT
			std::istringstream iss(att);
			std::tm t;
			t.tm_gmtoff = 0;
			t.tm_zone = "GMT";
			t.tm_isdst = 0;
			std::getline(iss, att, ',');
			iss >> t.tm_mday;
			iss.ignore();
			iss >> t.tm_mon;
			iss.ignore();
			iss >> t.tm_year;
			t.tm_year -= 1900;
			iss >> t.tm_hour;
			iss.ignore();
			iss >> t.tm_min;
			iss.ignore();
			iss >> t.tm_sec;
			expires = std::mktime(&t);
		}
		else if(att == "Secure")
			secure = true;
		else if(att == "HttpOnly")
			httponly = true;
		iss >> std::ws;
	}
}

bool cookie::is_expired()
{
	return !expires > time(0);
}

str urlencode(const str& url)
{
	static const str plain = "";
	std::ostringstream oss;
	str::const_iterator i = url.begin();
	for(; i != url.end(); ++i)
	{
		if(isalnum(*i) || plain.find(*i) != str::npos) { oss << *i; }
		else { oss << "%" << std::hex << int(*i); }
	}
	return oss.str();
}

// TODO: implement domain checking
std::string get_cookie_header(const cookie_jar& cookies, const str& /*domain*/)
{
//	bug_func();
	std::ostringstream oss;
	if(!cookies.empty())
	{
		oss << "Cookie:";
		str sep = " ";
		for(const cookie_pair& c: cookies)
		{
			oss << sep << c.second.key << "=" << c.second.val;
			sep = "; ";
		}
	}
	return oss.str();
}

std::ostream& write_cookie_headers(std::ostream& os, const cookie_jar& cookies)
{
	str sep;
	for(auto&& c: cookies)
	{
		os << sep << "Cookie: " << c.second.key << "=" << c.second.val;
		sep = "\r\n";
	}
	return os;
}



std::istream& read_http_headers(std::istream&is, header_map& headers)
{
//	bug_func();
	str line;

	if(!std::getline(is, line))
		return is;

	str key, val;
	std::istringstream iss;

	while(is && std::getline(is, line) && !trim(line).empty())
	{
		iss.clear();
		iss.str(line);
		if(iss >> key && std::getline(iss, val))
			headers.insert(std::make_pair(lower(trim(key, " \t\r\n:")), trim(val)));
	}
	return is;
}

std::istream& read_chunked_encoding(std::istream& is, str& data)
{
	data.clear();
	size_t len = 0;
	while(is >> std::hex >> len && len)
	{
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for(char c; len && is.get(c); --len)
			data += c;
	}
	return is;
}

std::istream& read_compress_encoding(std::istream& is, str& data)
{
	data.clear();
	log("ERROR: compress encoding not implemented");
	return is;
}

std::istream& read_deflate_encoding(std::istream& is, str& data)
{
	data.clear();
	log("ERROR: deflate encoding not implemented");
	return is;
}

std::istream& read_gzip_encoding(std::istream& is, str& data)
{
	data.clear();
	log("ERROR: gzip encoding not implemented");
	return is;
}

std::istream& read_http_response_data(std::istream&is, const header_map& headers, str& data)
{
	for(const net::header& h: headers)
	{
		if(h.first == "transfer-encoding")
		{
			if(h.second == "chunked")
				return read_chunked_encoding(is, data);
			else if(h.second == "compress" || h.second == "x-compress")
				return read_compress_encoding(is, data);
			else if(h.second == "deflate")
				return read_deflate_encoding(is, data);
			else if(h.second == "gzip" || h.second == "x-gzip")
				return read_gzip_encoding(is, data);
		}
		else if(h.first == "content-length")
		{
			size_t len;
			if(std::istringstream(h.second) >> len)
			{
//				std::ostringstream oss;
				data.clear();
				for(char c; len && is.get(c); --len)
					data += c;
//				data = oss.str();
				return is;
			}
		}
	}
	return is;
}

std::istream& read_http_cookies(std::istream& is, const header_map& headers, cookie_jar& cookies)
{
	for(const header& h: headers)
	{
		if(h.first == "set-cookie")
		{
			cookie c(h.second);
			cookies[c.key] = c;
		}
	}
	return is;
}

// HTML

static const str_map ents =
{
	{"&lt;", "<"}
	, {"&#60;", ">"}
	, {"&gt;", ">"}
	, {"&#61;", ">"}
	, {"&nbsp;", " "}
	, {"&#160;", " "}
	, {"&amp;", "&"}
	, {"&#38;", "&"}
	, {"&cent;", "¢"}
	, {"&#162;", "¢"}
	, {"&pound;", "£"}
	, {"&#163;", "£"}
	, {"&euro;", "€"}
	, {"&#8364;", "€"}
	, {"&sect;", "§"}
	, {"&#167;", "§"}
	, {"&copy;", "©"}
	, {"&#169;", "©"}
	, {"&reg;", "®"}
	, {"&#174;", "®"}
	, {"&trade;", "™"}
	, {"&#8482;", "™"}
	, {"&quot;", "\""}
};

str fix_entities(std::string s)
{
//	bug_func();
	str::size_type p;
	for(const str_map_vt& e: ents)
		while((p = s.find(e.first)) != str::npos)
			s.replace(p, e.first.size(), e.second);
	return s;
}

str urldecode(std::string s)
{
//	bug_func();
	static str_map urlism;
	if(urlism.empty())
	{
		for(size_t i = 0; i < 256; ++i)
		{
			std::ostringstream oss;
			oss.fill('0');
			oss << "%" << std::setw(2) << std::hex << i;
			//std::cout << "adding: " << oss.str() << '\n';

			urlism[lower(oss.str())] = str(1, char(i));
			urlism[upper(oss.str())] = str(1, char(i));
		}
	}
	str::size_type p;
	for(const str_map_vt& e: urlism)
	{
		while((p = s.find(e.first)) != str::npos)
		{
			//std::cout << "found: " << e.first << '\n';
			s.replace(p, e.first.size(), e.second);
		}
	}
	return s;
}

std::istream& read_tag(std::istream& is, std::ostream& os, const str& tag)
{
	bool done = false;
	str s;
	while(!done && std::getline(is, s, '<'))
	{
		if(std::getline(is, s, '>') && s.find(tag) == 0)
		{
			if(std::getline(is, s, '<'))
				os << s;

			while(std::getline(is, s, '>') && s.find("/" + tag) == str::npos)
			{
				os << '<' << s << '>';
				if(std::getline(is, s, '<'))
					os << s;
			}
			done = true;
		}
	}
	return is;
}

std::istream& read_tag_by_att(std::istream& is, std::ostream& os, const str& tag, const str& att, const str& val)
{
	bool done = false;
	str s;
	const str att1 = att + "='" + val + "'";
	const str att2 = att + "=\"" + val + "\"";

	while(!done && std::getline(is, s, '<'))
	{
		if(std::getline(is, s, '>') && s.find(tag) == 0 && (s.find(att1) != str::npos || s.find(att2) != str::npos))
		{
			if(std::getline(is, s, '<')) os << s;

			while(std::getline(is, s, '>') && s.find("/" + tag) == str::npos)
			{
				os << '<' << s << '>';
				if(std::getline(is, s, '<')) os << s;
			}
			done = true;
		}
	}
	return is;
}

std::istream& read_element(std::istream& is, str& tag)
{
	tag.clear();
	str t;
	if(is.peek() != '<') // read up to tag.
	{
		if(std::getline(is, t, '<')) // between tags
		{
			tag += t;
			is.putback('<');
		}
		return is;
	}

	siz d = 0;
	while(is)
	{
		if(std::getline(is, t, '<')) // between tags
			tag += t;

		if(std::getline(is, t, '>'))
		{
			if(!t.empty() && t[0] != '/' && t[t.size() - 1] != '/') // open tag
				++d;
			else if(!t.empty() && t[0] == '/') // close tag
			{
				if(--d == 0)
				{
					tag += "<" + t + ">";
					return is;
				}
			}
			tag += "<" + t + ">";
		}
	}
	return is;

}

void html_to_text(std::istream& i, std::ostream& o)
{
	for(std::string s; std::getline(i, s, '<'); std::getline(i, s, '>'))
		o << urldecode(fix_entities(s));
}

str html_to_text(const str& html)
{
	std::istringstream i(html);
	std::ostringstream o;
	html_to_text(i, o);
	return o.str();
}

bool hostname_to_ip(const str& hostname , str& ip)
{
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(hostname.c_str(), "http", &hints, &servinfo)) != 0)
		return false;

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		h = (sockaddr_in*) p->ai_addr;
		ip = inet_ntoa(h->sin_addr);
	}

	freeaddrinfo(servinfo); // all done with this structure
	return true;
}

namespace bin {

typedef unsigned char byte;

// create overloads of these functions so that they
// can be selected by type in templated functions
inline uint16_t hton(uint16_t i) { return htons(i); }
inline uint32_t hton(uint32_t i) { return htonl(i); }
inline uint16_t ntoh(uint16_t i) { return ntohs(i); }
inline uint32_t ntoh(uint32_t i) { return ntohl(i); }

template<typename Type>
byte* insert(byte* data, Type type)
{
	type = hton(type);
	for(size_t i(0); i < sizeof(Type); ++i)
		data[i] = reinterpret_cast<byte*>(&type)[i];
	return data + sizeof(Type);
}

byte* insert(byte* data, char const* s)
{
	while((*reinterpret_cast<char*>(data++) = *s++)) {}
	return data;
}

byte* insert(byte* data, const std::string& s)
{
	return insert(data, s.c_str());
}

/**
 * General routing to extract aligned integral types
 * from the UDP packet.
 *
 * @param data Pointer into the UDP packet data
 * @param type Integral type to extract
 *
 * @return data pointer advanced to next position after extracted integral.
 */
template<typename Type>
byte const* extract(byte const* data, Type& type)
{
	static_assert(std::is_integral<Type>::value, "");
	// This union will ensure the integral data type is correctly aligned
	union tx_t
	{
		byte cdata[sizeof(Type)];
		Type tdata;
	} tx;

	for(size_t i(0); i < sizeof(Type); ++i)
		tx.cdata[i] = data[i];

	type = ntoh(tx.tdata);

	return data + sizeof(Type);
}

/**
 * If strings are null terminated in the buffer then this could be used to extract them.
 *
 * @param data Pointer into the UDP packet data
 * @param s std::string type to extract
 *
 * @return data pointer advanced to next position after extracted std::string.
 */
byte const* extract(byte const* data, std::string& s)
{
	s.assign((char const*)data, std::strlen((char const*)data));
	return data + s.size() + 1;
}

/**
 *  Function to parse entire UDP packet
 *
 * @param data The entire UDP packet data
 */
void read_data(byte const* const data)
{
	std::string s0;
	uint16_t i1;
	std::string s1;
	uint32_t i2;
	std::string s2;

	byte const* p;

	p = extract(data, s0);
	p = extract(p, i1); // p contains next position to read
	p = extract(p, s1);
	p = extract(p, i2);
	p = extract(p, s2);

	con("s0: " << s0);
	con("i1: " << i1);
	con("s1: " << s1);
	con("i2: " << i2);
	con("s2: " << s2);
}

void write_data(byte* const data)
{
	uint16_t i1 = 123;
	std::string s1 = "456";
	uint32_t i2 = 789;
	std::string s2 = "012";

	con("i1: " << i1);
	con("s1: " << s1);
	con("i2: " << i2);
	con("s2: " << s2);

	byte* p;

	p = insert(data, "A");
	p = insert(p, i1); // p contains next position to read
	p = insert(p, s1);
	p = insert(p, i2);
	p = insert(p, s2);
}

} // bin

}} // sookee::net
