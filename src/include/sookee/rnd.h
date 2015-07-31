/*
 *
 *  Created on: 6 Jul 2014
 *      Author: SooKee oasookee@gmail.com
 */

#ifndef LIBSOOKEE_RND_H_
#define LIBSOOKEE_RND_H_

#include <random>

namespace sookee { namespace rnd {

// __cplusplus 199711L
// __cplusplus 201103L
// __cplusplus 201300L ??
// __cplusplus 201402L

#if __cplusplus >= 201103L
#else
#define thread_local
#endif

inline
std::mt19937& engine()
{
	thread_local static std::mt19937 engine {std::random_device{}()};
	return engine;
}

using seed_type = decltype(engine()());

inline
void seed(seed_type s)
{
	engine().seed(s);
}

template<typename Integral>
Integral randint(Integral min, Integral max)
{
	thread_local static std::uniform_int_distribution<Integral> dist;
	typename std::uniform_int_distribution<Integral>::param_type p{min, max};
	return dist(engine(), p);
}

template
	<
	  typename Type = int
	, typename Generator = std::mt19937
	, typename Dist = typename std::conditional<std::is_integral<Type>::value
	, std::uniform_int_distribution<Type>
	, std::uniform_real_distribution<Type>>::type
	>
class PRNG
{
	using rd = std::random_device;
	using param_type = typename Dist::param_type;
	std::seed_seq ss;
	Generator gen;
	Dist dis;

public:
	PRNG(): ss({rd{}(), rd{}(), rd{}(), rd{}()}), gen(ss) {}

	Type get(Type from, Type to)
	{
		return dis(gen, param_type{from, to});
	}

	/**
	 * Chances of success. This function will return true
	 * num times out of denom.
	 * @param num
	 * @param denom
	 * @return
	 */
	bool chances(Type num, Type denom) { return num > get(1, denom); }

	/**
	 * Percentage chance of success. This function returns true
	 * num percent of the time.
	 * @param num
	 * @return
	 */
	bool pc(Type num) { return chances(num, 100); }
};

template<typename Type = std::int32_t>
using PRNG_32 = PRNG<Type>;

using PRNG_32U = PRNG<std::uint32_t>;

template<typename Type = std::int64_t>
using PRNG_64 = PRNG<Type, std::mt19937_64>;

using PRNG_64U = PRNG<std::uint32_t, std::mt19937_64>;

enum class type {lower, upper, both};

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
	unsigned int n = 0;

public:
	lcg(int n = 0): n(n) {}

	void seed(int n) { this->n = n; }

	int rand()
	{
		n = (214013 * n + 2531011);
		return (n >> 16) & 0x7FFF;
	}
};

}} // ::sookee::rnd

namespace soo { using namespace sookee::rnd; }

#endif // LIBSOOKEE_RND_H_
