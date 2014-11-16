#ifndef LIBSOOKEE_XIO_H_
#define LIBSOOKEE_XIO_H_
/*
 *  Created on: 04 Oct 2014
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee oasookee@gmail.com                     |
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

#include <cstdio>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <istream>
#include <ostream>

#include <ext/stdio_filebuf.h>

namespace sookee { namespace xio {

namespace client { typedef int socket; }
namespace server { typedef int socket; }

using namespace __gnu_cxx;

class xstream
: public std::istream
, public std::ostream
{
	stdio_filebuf<char> fb;
	int fd = 0;

public:
	xstream(int fd): fb(fd, std::ios::in|std::ios::out) {}
	~xstream() { close(fd); }
};

class xio
{
protected:
	struct close_scoper
	{
		int fd;
		close_scoper(int fd): fd(fd) {}
		~close_scoper() { close(fd); }
	};

	bool log_errno() const
	{
		std::cerr << "ERROR: " << strerror(errno) << std::endl;
		return false;
	}
};

class xserver
: virtual public xio
{
	server::socket ss = 0;

public:
	xserver() {}
	~xserver() { close(ss); }

	bool open(const std::string name)
	{
		if((ss = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
			return log_errno();

		close_scoper closer(ss);

		sockaddr_un addr;
		std::memset((void*)&addr, 0, sizeof(addr));

		addr.sun_family = AF_UNIX;
		std::strncpy(addr.sun_path, name.c_str(), sizeof(addr.sun_path) - 1);

		unlink(name.c_str());

		if(bind(ss, (struct sockaddr*) &addr, sizeof(addr)) == -1)
			return log_errno();

		if(listen(ss, 5) == -1)
			return log_errno();

		return true;
	}

	bool accept(client::socket& cs)
	{
		if((cs = ::accept(ss, 0, 0)) == -1)
			return log_errno();
		return true;
	}
};

class xclient
: virtual public xio
{
public:
	xclient() {}
	~xclient() { }

	bool open(const std::string name, client::socket& cs)
	{
		sockaddr_un addr;
		if((cs = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
			return log_errno();

		close_scoper closer(cs);

		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		std::strncpy(addr.sun_path, name.c_str(), sizeof(addr.sun_path) - 1);

		if(connect(cs, (struct sockaddr*) &addr, sizeof(addr)) == -1)
			return log_errno();

		return true;
	}
};

int example()
{
	xserver server;

	if(!server.open(".socket"))
		return -1;

	//char buf[1024];
	client::socket cs;
	std::string message;
	//std::streamsize len;

	while(server.accept(cs))
	{
		xstream xs(cs);

		if(!std::getline(xs, message))
		{
			std::cerr << "ERROR: " << strerror(errno) << std::endl;
			continue;
		}

//		message.clear();
//		while((len = xs.readsome(buf, 100)) > 0)
//			message.append(buf, len);

		if(message == "RQDATA")
			xs << "some data" << std::endl;
		else
			std::cerr << "ERROR: unknown message: " << message << std::endl;
	}
	return 0;
}

}} // sookee::xio

namespace soo { using namespace sookee::xio; }

#endif // LIBSOOKEE_XIO_H_
