/*
 *
 *  Created on: 6 Jul 2014
 *      Author: SooKee oasookee@gmail.com
 */

#ifndef RND_H_
#define RND_H_

#include <random>

namespace sookee { namespace rnd {

template<typename RandomEngine = std::default_random_engine>
int rnd(int a, int b, RandomEngine e = RandomEngine())
{
	static std::uniform_int_distribution<> d;
	std::uniform_int_distribution<>::param_type p{a, b};

    return d(e, p);
}

//std::random_device rd;
//std::default_random_engine eng{rd()};
//std::uniform_int_distribution<> dist;
//int n = dist(eng, decltype(dist)::param_type{0, A});

}} // ::sookee::rnd

namespace soo { using namespace sookee::rnd; }

#endif /* RND_H_ */
