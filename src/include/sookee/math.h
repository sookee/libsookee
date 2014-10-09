/*
 *  Created on: 08 Oct 2014
 *	  Author: oasookee@gmail.com
 */

#ifndef _LIBSOOKEE_MATH_H_
#define _LIBSOOKEE_MATH_H_

namespace sookee { namespace math {

template<typename Integral>
bool is_prime(Integral n)
{
	if(n == 1)
		return false;

	if(n == 2)
		return true;

	if(n % 2 == 0)
		return false;

	for(size_t i = 3; i * i <= n; i += 2)
		if(n % i == 0)
			return false;

	return true;
}

}} // sookee::math

namespace soo { using namespace sookee::math; }

#endif // _LIBSOOKEE_MATH_H_
