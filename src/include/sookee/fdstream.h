#ifndef LIBSOOKEE_FDSTREAM_H
#define LIBSOOKEE_FDSTREAM_H

// This code is modification of code (C) Copyright Nicolai M. Josuttis 2001
// provided with the following Copyright Notice:

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

// It is released under the same terms.
#include <istream>
#include <ostream>
#include <streambuf>
// for EOF:
#include <cstdio>
// for memmove():
#include <cstring>
#include <memory>

// low-level read and write functions
#ifdef _MSC_VER
# include <io.h>
#else
# include <unistd.h>
//extern "C" {
//    int write (int fd, const char* buf, int num);
//    int read (int fd, char* buf, int num);
//}
#endif

// BEGIN namespace BOOST
namespace sookee { namespace net {

/************************************************************
 * fdostream
 * - a stream that writes on a file descriptor
 ************************************************************/

class fdoutbuf
: public std::streambuf
{
protected:
	int fd = -1;

public:
	fdoutbuf(int fd = -1): fd(fd) {}
	void set_fd(int fd) { this->fd = fd; }

protected:
	// write one character
	int_type overflow(int_type c) override
	{
		if(c != EOF)
		{
			char z = c;
			if(write(fd, &z, 1) != 1)
			{
				return EOF;
			}
		}
		return c;
	}
	// write multiple characters
	std::streamsize xsputn(const char* s, std::streamsize num) override
	{
		return write(fd, s, num);
	}
};

class fdostream
: public std::ostream
{
protected:
	fdoutbuf buf;

public:
	fdostream(int fd = -1): std::ostream(0), buf(fd)
	{
		rdbuf(&buf);
	}

	void set_fd(int fd) { clear(); buf.set_fd(fd); }
};

/************************************************************
 * fdistream
 * - a stream that reads on a file descriptor
 ************************************************************/

class fdinbuf
: public std::streambuf
{
protected:
	int fd;

protected:
	/* data buffer:
	 * - at most, pbSize characters in putback area plus
	 * - at most, bufSize characters in ordinary read buffer
	 */
	static const int pbSize = 4;        // size of putback area
	static const int bufSize = 1024;    // size of the data buffer
	char buffer[bufSize + pbSize];        // data buffer

public:
	/* constructor
	 * - initialize file descriptor
	 * - initialize empty data buffer
	 * - no putback area
	 * => force underflow()
	 */
	fdinbuf(int fd = -1): fd(fd)
	{
		setg(buffer + pbSize,     // beginning of putback area
		buffer + pbSize,     // read position
		buffer + pbSize);    // end position
	}

	void set_fd(int fd) { this->fd = fd; }

protected:
	// insert new characters into the buffer
	virtual int_type underflow()
	{
#ifndef _MSC_VER
		using std::memmove;
#endif

		// is read position before end of buffer?
		if(gptr() < egptr())
		{
			return traits_type::to_int_type(*gptr());
		}

		/* process size of putback area
		 * - use number of characters read
		 * - but at most size of putback area
		 */
		int numPutback;
		numPutback = gptr() - eback();
		if(numPutback > pbSize)
		{
			numPutback = pbSize;
		}

		/* copy up to pbSize characters previously read into
		 * the putback area
		 */
		memmove(buffer + (pbSize - numPutback), gptr() - numPutback,
			numPutback);

		// read at most bufSize new characters
		int num;
		num = read(fd, buffer + pbSize, bufSize);
		if(num <= 0)
		{
			// ERROR or EOF
			return EOF;
		}

		// reset buffer pointers
		setg(buffer + (pbSize - numPutback),   // beginning of putback area
		buffer + pbSize,                // read position
		buffer + pbSize + num);           // end of buffer

		// return next character
		return traits_type::to_int_type(*gptr());
	}
};

class fdistream: public std::istream
{
protected:
	fdinbuf buf;

public:
	fdistream(int fd = -1): std::istream(0), buf(fd)
	{
		rdbuf(&buf);
	}

	void set_fd(int fd) { clear(); buf.set_fd(fd); }
};

class fdstream: public std::iostream//public fdostream, public fdistream
{
	std::unique_ptr<fdinbuf> ibuf;
	std::unique_ptr<fdoutbuf> obuf;
public:
	fdstream(int fd = -1)
	: ibuf(new fdinbuf(fd))
	, obuf(new fdoutbuf(fd))
	{
		std::istream::rdbuf(ibuf.get());
		std::ostream::rdbuf(obuf.get());
	}

	fdstream(fdstream&& fds)
	: ibuf(std::move(fds.ibuf))
	, obuf(std::move(fds.obuf))
//	: ibuf(fds.ibuf.release())
//	, obuf(fds.obuf.release())
	{
		std::istream::rdbuf(ibuf.get());
		std::ostream::rdbuf(obuf.get());
		fds.std::istream::rdbuf(new fdinbuf(-1));
		fds.std::ostream::rdbuf(new fdoutbuf(-1));
		fds.setstate(std::ios::failbit);
	}

	void set_fd(int fd)
	{
		clear();
		if(ibuf)
			ibuf->set_fd(fd);
		if(obuf)
			obuf->set_fd(fd);
	}
};

}
} // ::sookee::net

#endif // LIBSOOKEE_FDSTREAM_H
