#ifndef _LIBSOOKEE_TYPED_INTEGRAL_H_
#define _LIBSOOKEE_TYPED_INTEGRAL_H_
/*
 *  Created on: 15 Jul 2014
 *	  Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee oasookee@gmail.com					   |
'------------------------------------------------------------------'

This macro is modified from the boose BOOST_STRONG_TYPEDEF macro.

Here is the license:

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

http://www.boost.org/LICENSE_1_0.txt

'-----------------------------------------------------------------*/

namespace sookee { namespace types {

#define TYPED_INTEGRAL(Type, Def) \
class Def \
{ \
	static_assert(std::is_integral<Type>::value, "Must be integral type"); \
	Type t; \
public: \
	Def(): t(0) {}; \
	explicit Def(const Type& t): t(t) {}; \
	Def(const Def& d): t(d.t) {} \
	Def& operator=(const Def& d) { t = d.t; return *this; } \
	Def& operator=(const Type& t) { this->t = t; return *this; } \
	operator const Type&() const {return t; } \
	operator Type&() { return t; } \
	bool operator==(const Def& d) const { return t == d.t; } \
	bool operator<(const Def& d) const { return t < d.t; } \
}

}} // sookee::types

namespace soo { using namespace sookee::types; }

#endif // _LIBSOOKEE_TYPED_INTEGRAL_H_