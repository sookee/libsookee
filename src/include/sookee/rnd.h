/*
 *
 *  Created on: 6 Jul 2014
 *      Author: SooKee oasookee@gmail.com
 */

#ifndef RND_H_
#define RND_H_

#include <random>

namespace sookee { namespace rnd {

inline
std::mt19937& engine()
{
	thread_local static std::random_device rd {};
	thread_local static std::mt19937 engine {rd()};
	return engine;
}

using seed_type = decltype(engine()());

inline
void seed(seed_type s)
{
	engine().seed(s);
}

template<typename Integral>
Integral randint(Integral min, Integral max)// -> decltype(min + max)
{
//	using Integral = decltype(min + max);
	thread_local static std::uniform_int_distribution<Integral> dist;
	typename std::uniform_int_distribution<Integral>::param_type p{min, max};
	return dist(engine(), p);
}

//template<typename Integral>
//Integral integral(Integral b, Integral e)
//{
//	static std::uniform_int_distribution<Integral> d;
//	typename std::uniform_int_distribution<Integral>::param_type p{b, e};
//	return d(mt_rng(), p);
//}
//
//template<typename Real>
//Real real(Real b, Real e)
//{
//	static std::uniform_real_distribution<Real> d;
//	typename std::uniform_real_distribution<Real>::param_type p{b, e};
//	return d(mt_rng(), p);
//}

enum class type { lower, upper, both };

inline
std::string random_alphanum(size_t length, type t = type::both)
{
    static const std::string anl =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";

    static const std::string anu =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static const std::string anb =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    const std::string& an = t == type::lower ? anl : t == type::upper ? anu : anb;


//    static std::mt19937 rg(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<> pick(0, (int)an.size() - 1);

    std::string s;

    s.reserve(length);

    while(length--)
        s += an[pick(engine())];

    return s;
}

//template<typename RandomEngine = std::default_random_engine>
//int rnd(int a, int b, RandomEngine e = RandomEngine())
//{
//	static std::uniform_int_distribution<> d;
//	typename std::uniform_int_distribution<>::param_type p{a, b};
//
//    return d(e, p);
//}

//std::random_device rd;
//std::default_random_engine eng{rd()};
//std::uniform_int_distribution<> dist;
//int n = dist(eng, decltype(dist)::param_type{0, A});

// Linear Congruential Generator
class lcg
{
	unsigned int seed = 0;

public:
	void srand(int seed) { this->seed = seed; }

	int rand()
	{
		seed = (214013 * seed + 2531011);
		return (seed >> 16) & 0x7FFF;
	}
};

}} // ::sookee::rnd

namespace soo { using namespace sookee::rnd; }

#endif /* RND_H_ */
