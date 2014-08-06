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

#include <unicode/utypes.h>
#include <unicode/ucnv.h>

namespace sookee { namespace i18n {

i18n::utf8 i18n::to_utf8(const i18n::utf32& utf32)
{
	UErrorCode status = U_ZERO_ERROR;
	char target[1024];
	int32_t len = ucnv_convert("UTF-8", "UTF-32", target, 1024, (const char*)utf32.data(), utf32.size() * sizeof(char32_t), &status);
//	con("status: " << status);
	return i18n::utf8(target, len);
}

i18n::utf32 i18n::to_utf32(const i18n::utf8& utf8)
{
	UErrorCode status = U_ZERO_ERROR;
	char target[1024];
	int32_t len = ucnv_convert("UTF-32", "UTF-8", target, 1024, utf8.data(), utf8.size(), &status);
//	con("status: " << status);
	return i18n::utf32((char32_t*) target, len / sizeof(char32_t));
}


}} // sookee::i18n
