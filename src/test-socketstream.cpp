/*
 *  Created on: 2 Nov 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/bug.h>
#include <sookee/log.h>
//#include <sookee/socketstream.h>
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
#include <arpa/inet.h>

#include <sookee/types.h>
#include <ext/stdio_filebuf.h>

using namespace sookee::bug;
using namespace sookee::log;
//using namespace sookee::net;

namespace sookee { namespace net {

using namespace __gnu_cxx;
using namespace sookee::types;

template<typename Char>
class basic_socketstream
: public std::basic_iostream<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_iostream<char_type> stream_type;
	typedef __gnu_cxx ::stdio_filebuf<char_type> buf_type;

protected:
	//buf_type buf = nullptr;

public:
	basic_socketstream(): stream_type(nullptr) {}
	basic_socketstream(int fd
		, const std::ios::openmode& mode = std::ios::in|std::ios::out)
	: stream_type(new buf_type(fd, mode)) {}
	basic_socketstream(const str& host, const str& port
		, const std::ios::openmode& mode = std::ios::in|std::ios::out)
	: stream_type(nullptr)
	{
		open(host, port, mode);
	}

	~basic_socketstream() { close(); }

	void open(const str& host, const in_port_t port
		, const std::ios::openmode& mode = std::ios::in|std::ios::out)
	{
		open(host, std::to_string(port), mode);
	}

	stream_type& open(const str& host, const str& port
		, const std::ios::openmode& mode = std::ios::in|std::ios::out)
	{
		addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
		hints.ai_socktype = SOCK_STREAM;

		addrinfo* res;
		if(int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
		{
			std::basic_iostream<Char>::setstate(std::ios::failbit);
			log("ERROR: getting addres info: " << gai_strerror(status));
			return *this;
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
			return *this;
		}

		delete std::basic_iostream<Char>::rdbuf(new buf_type(sd, mode));

		if(!std::basic_iostream<Char>::rdbuf())
			std::basic_iostream<Char>::setstate(std::ios::failbit); // memory fail
		return *this;
	}

	void close()
	{
		delete std::basic_iostream<Char>::rdbuf(0);
	}
};

typedef basic_socketstream<char> socketstream;
typedef basic_socketstream<wchar_t> wsocketstream;

class str_net
{
	int sock = -1;
	socketstream ss;

	bool done = false;

public:
	str_net(): sock(::socket(PF_INET, SOCK_STREAM, 0)) {}

	bool bind(const str& host, const str& port)
	{
		bug_func();
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(std::stol(port));
		addr.sin_addr.s_addr = inet_addr(host.c_str());
		return ::bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != -1;
	}

	template<typename Functor>
	bool accept(const str& host, const str& port, Functor func)
	{
		bug_func();
		if(!bind(host, port))
		{
			log("ERROR: " << std::strerror(errno));
			return false;
		}

		int option = 1;
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)))
		{
			log("ERROR: " << std::strerror(errno));
			return false;
		}

		if(::listen(sock, 10) == -1)
		{
			log("ERROR: " << std::strerror(errno));
			return false;
		}

		done = false;

		while(!done)
		{
			sockaddr connectee;
			socklen_t connectee_len = sizeof(sockaddr);
			int cs;

			while(!done)
			{
				while(!done && (cs = ::accept4(sock, &connectee, &connectee_len, SOCK_NONBLOCK)) == -1)
				{
					if(!done)
					{
						if(errno ==  EAGAIN || errno == EWOULDBLOCK)
							std::this_thread::sleep_for(std::chrono::seconds(1));
						else
						{
							log("ERROR: " << strerror(errno));
							::close(cs);
							continue;
						}
					}
				}

				if(!done)
				{
					if(connectee.sa_family != AF_INET)
					{
						log("WARN: only accepting connections for ipv4");
						::close(cs);
						continue;
					}
//					if(accepts.empty() || accepts.count(((sockaddr_in*) &connectee)->sin_addr.s_addr))
					{
						socketstream css(cs);
						func(css);
					}
				}
			}
		}
		return true;
	}

	void stop() { done = true; }

};

}} // sookee::net

namespace soo { using namespace sookee::net; }

#include <algorithm>

int main()
{
	sookee::net::socketstream ss;

	ss.open("openarena.ws", "80");

	bug_var(ss);

	ss << "GET /smfnews.php" << " HTTP/1.1\r\n";
	ss << "Host: openarena.ws" << "\r\n";
	ss << "User-Agent: libsookee: " << "0.0" << "\r\n";
	ss << "Accept: text/html" << "\r\n";
	ss << "\r\n" << std::flush;

	for(char c; ss.get(c); std::cout.put(c)) {}

	std::copy(std::istream_iterator<char>(ss), std::istream_iterator<char>()
		, std::ostream_iterator<char>(std::cout));
}
