#ifndef LIBSOOKEE_I18N_H_
#define LIBSOOKEE_I18N_H_

/*
 *  Created on: Aug 2, 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <string>

namespace sookee { namespace i18n {

class i18n
{
public:
	struct utf8: std::string { using std::string::string; };
	struct utf32: std::u32string { using std::u32string::u32string; };

	static i18n::utf8 to_utf8(const i18n::utf32& utf32);
	static i18n::utf32 to_utf32(const i18n::utf8& utf8);
};

}} // ::sookee::i18n

namespace soo { using namespace sookee::i18n; }

#endif // LIBSOOKEE_I18N_H_
