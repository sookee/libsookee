#ifndef LIBSOOKEE_I18N_H_
#define LIBSOOKEE_I18N_H_

/*
 *  Created on: Aug 2, 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <string>
#include <uchar.h>

namespace sookee { namespace i18n {

std::string to_utf8(const std::u32string& utf32);
std::u32string to_utf32(const std::string& utf8);

/**
 * Multibyte to UTF-32
 * @param mb Multibyte string in the current locale
 * @return UTF-32 string
 */
std::u32string mb_to_utf32(const std::string& mb);

/**
 * UTF-32 string to multibyte string
 * @param utf32 UTF-32 string
 * @return multibyte representation in the current locale
 */
std::string utf32_to_mb(const std::u32string& utf32);

}} // ::sookee::i18n

namespace soo { using namespace sookee::i18n; }

#endif // LIBSOOKEE_I18N_H_
