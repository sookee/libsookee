/*
 *  Created on: 2 Aug 2014
 *      Author: SooKee <oasookee@gmail.com>
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

#include <sookee/i18n.h>
#include <iconv.h>
#include <cstring>
#include <memory>
#include <wchar.h>

//#include <unicode/utypes.h>
//#include <unicode/ucnv.h>

namespace sookee { namespace i18n {

//std::string to_utf8(const std::u32string& s)
//{
//	UErrorCode status = U_ZERO_ERROR;
//	char buf[1024];
//	int32_t len = ucnv_convert("UTF-8", "UTF-32", buf, 1024, (const char*)s.data(), s.size() * 4, &status);
////	con("status: " << status);
//	return std::string(buf, len);
//}
//
//std::u32string to_utf32(const std::string& utf8)
//{
//	UErrorCode status = U_ZERO_ERROR;
//	char target[1024];
//	int32_t len = ucnv_convert("UTF-32", "UTF-8", target, 1024, utf8.data(), utf8.size(), &status);
////	con("status: " << status);
//	return std::u32string((char32_t*) target, len / sizeof(char32_t));
//}

std::string to_utf8(std::u32string utf32)
{
	std::string utf8;
	utf8::utf32to8(utf32.begin(), utf32.end(), std::back_inserter(utf8));
	return utf8;
}

std::u32string to_utf32(const std::string& utf8)
{
	std::u32string utf32;
	utf8::utf8to32(utf8.begin(), utf8.end(), std::back_inserter(utf32));
	return utf32;
}

// Untested iconv version
std::string to_utf8_xxx(std::u32string utf32)
{
	iconv_t cd;
	if((cd = iconv_open("UTF-8", "UTF-32")) == iconv_t(-1))
		throw std::runtime_error(std::strerror(errno));

	std::unique_ptr<char[]> buf(new char[utf32.size() * 4]);

	auto inbuf = (char*)&utf32[0];
	auto inbytesleft = utf32.size() * sizeof(std::u32string::value_type);
	auto outbuf = buf.get();
	auto outbytesleft = utf32.size() * 4;

	iconv(cd, nullptr, nullptr, nullptr, nullptr);

	if(iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft) == std::size_t(-1))
		throw std::runtime_error(std::strerror(errno));

	return std::string(buf.get(), outbuf);
}

std::u32string mb_to_utf32(const std::string& mb)
{
	std::u32string utf32;

	char32_t pc32;
	mbstate_t ps;

	size_t len;
	std::string::size_type pos;

	mbrlen(0, 0, &ps);
	for(pos = 0; (len = mbrtoc32(&pc32, mb.c_str() + pos, MB_CUR_MAX, &ps) != 0); pos += len)
	{
		if(pos == size_t(-1) || pos == size_t(-2) || pos == size_t(-3))
			break;
		utf32 += pc32;
	}

	return utf32;
}

std::string utf32_to_mb(const std::u32string& utf32)
{
	std::string mb;

	char pmb[4];
	mbstate_t ps;

	size_t len;

	mbrlen(0, 0, &ps);
	for(size_t pos = 0; pos < utf32.size(); ++pos)
	{
		if((len = c32rtomb(pmb, utf32[pos], &ps)) <= 4)
			mb.append(pmb, len);
	}

	return mb;
}

}} // sookee::i18n
