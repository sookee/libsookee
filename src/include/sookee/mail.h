/*
 * mail.h
 *
 *  Created on: Jan 16, 2013
 *      Author: oasookee@googlemail.com
 */

#ifndef _LIBSOOKEE_MAIL_H_
#define _LIBSOOKEE_MAIL_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2012 SooKee oasookee@gmail.com               |
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

#include <sookee/str.h>
#include <sookee/types.h>
#include <sookee/logrep.h>
#include <sookee/socketstream.h>

namespace sookee { namespace email {

using namespace sookee;
using namespace sookee::types;
using namespace sookee::string;

class SMTP
{
	const str host;
	const long port;

	bool tx(std::ostream& os, const str& data = "")
	{
		return os << data << "\r\n" << std::flush;
	}

	bool rx(std::istream& is, const str& code)
	{
		str reply;
		if(!sgl(is, reply))
		{
			log(strerror(errno));
			return false;
		}
		trim(reply);
		bug_var(reply);
		if(reply.find(code))
			return false;
		return true;
	}

public:
	// protocol
	str mailfrom;
	str rcptto;

	// headers
	str from;
	str to;
	str replyto;

	SMTP(const str& host = "localhost", long port = 25): host(host), port(port) {}

	bool sendmail(const str& subject, const str& data)
	{
		net::socketstream ss;

		str reply;

		std::time_t t = std::time(0);

		tm* timeinfo = localtime(&t);
		char buffer[80];

		siz size = strftime(buffer, 80, "%a, %d %b %Y %T %z", timeinfo);

		str time(buffer, size);
//		trim(time);

		ss.open(host, port);

		if(!rx(ss, "220"))
			return false;

		tx(ss, "HELO sookee.dyndns.org");
		if(!rx(ss, "250"))
			return false;

		// one or more
		{
			tx(ss, "MAIL FROM: " + mailfrom);
			if(!rx(ss, "250"))
				return false;

			// one or more
			{
				tx(ss, "RCPT TO: " + rcptto);
				if(!rx(ss, "250"))
					return false;
			}

			tx(ss, "DATA");
			if(!rx(ss, "354"))
				return false;
			tx(ss, "From: " + mailfrom);
			tx(ss, "To: " + rcptto);
			tx(ss, "Date: " + time);
			tx(ss, "Subject: " + subject);
			if(!replyto.empty())
				tx(ss, "Reply-To: " + replyto);
			tx(ss);
			tx(ss, data);
			tx(ss, ".");
			if(!rx(ss, "250"))
				return false;
		}

		tx(ss, "QUIT");
		if(!rx(ss, "221"))
			return false;

		ss.close();

		return true;
	}
};

//int main()
//{
//	SMTP smtp("localhost", 25);
//
//	smtp.mailfrom = "<noreply@sookee.dyndns.org>";
//	smtp.rcptto = "<oasookee@gmail.com>";
//
//	smtp.from = "No Reply " + smtp.mailfrom;
//	smtp.to = "SooKee " + smtp.rcptto;
//	smtp.replyto = "Skivvy <oasookee@gmail.com>";
//
//	smtp.sendmail("New World Order.", "Some thing great.");
//}


}} // sookee::email

namespace soo { using namespace sookee::email; }

#endif /* _LIBSOOKEE_MAIL_H_ */
