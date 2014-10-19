#ifndef LIBSOOKEE_RAD_PARSER_H_
#define LIBSOOKEE_RAD_PARSER_H_
/**
 * RAD PARSER = Rapid And Dangerous Parser
 */

#include <iostream>
#include <cmath>

typedef std::string str;
typedef std::size_t siz;
typedef float flt;
typedef const char* rad;

namespace sookee { namespace radp {

/**
 * Advance position by n places or to EOS
 * whichever comes first.
 * @param s
 * @param n
 * @return
 */
inline rad adv(rad s, siz n = 1)
{
	while(*s && n--)
		++s;
	return s;
}

/**
 * Parse to delim
 * @param s
 * @return
 */
inline rad pt(rad s, char delim = '\n')
{
	while(*s && *s != delim)
		++s;
	return s;
}

inline rad ps(rad s)
{
	while(*s == ' ')
		++s;
	return s;
}

/**
 * Parse Character
 * @param s
 * @param c
 * @return returned value == s on error (EOS)
 * else return value == s + 1
 */
inline rad pc(rad s, char& c)
{
	if(*s)
		c = *s++;
	return s;
}

inline rad pz(rad s, siz& i)
{
	for(i = 0; *s >= '0' && *s <= '9'; ++s)
		i = (i << 3) + (i << 1) + *s - '0';
	return s;
}

/**
 * Parse siz ignoring leading spaces
 * @param s
 * @param i
 * @return
 */
inline rad psz(rad s, siz& i)
{
	return pz(ps(s), i);
}

/**
 * Parse the unsigned part of an int (after the '-' sign
 * @param s
 * @param i
 * @return
 */
inline rad pu(rad s, int& i)
{
	for(i = 0; *s >= '0' && *s <= '9'; ++s)
		i = (i << 3) + (i << 1) + *s - '0';
	return s;
}

/**
 * Parse Integer
 * @param s pointer to integer data
 * @param i out parameter
 * @return
 */
inline rad pi(rad s, int& i)
{
	if(*s != '-')
		return pu(s, i);

	rad ss = s;

	s = pu(adv(s, 1), i);
	if(s == ss + 1)
		return ss; // error s unchanged
	i = -i;
	return s;
}

/**
 * Parse int ignoring leading spaces
 * @param s
 * @param i
 * @return
 */
inline rad psi(rad s, int& i)
{
	return pi(ps(s), i);
}

static const siz pow10[] =
{
	1u
	, 10u
	, 100u
	, 1000u
	, 10000u
	, 100000u
	, 1000000u
	, 10000000u
	, 100000000u
	, 1000000000u
	, 10000000000u
	, 100000000000u
	, 1000000000000u
	, 10000000000000u
	, 100000000000000u
	, 1000000000000000u
	, 10000000000000000u
	, 100000000000000000u
	, 1000000000000000000u
	, 10000000000000000000u
};

inline rad pf(rad s, float& i)
{
	rad x = s;
	int num;
	siz den;
	bool neg = false;

	if(*s == '-')
	{
		neg = true;
		s = adv(s, 1);
	}

	s = pu(s, num);

	if(*s != '.')
	{
		i = static_cast<float>(neg ? -num : num);
		return s;
	}

	rad p = pz(adv(s, 1), den);

	siz exp = p - adv(s, 1);

	if(exp >= sizeof(pow10))
		return x; // error

	i = static_cast<float>(num + (double(den) / double(pow10[exp])));

	if(neg)
		i = -i;

	return p;
}

/**
 * Parse float ignoring leading spaces
 * @param s
 * @param i
 * @return
 */
inline rad psf(rad s, float& i)
{
	return pf(ps(s), i);
}


/**
 * parse NOT space (word)
 *
 *
	for(rad p, s = ps(line.c_str()); (p = pw(s, w)) != s; s = ps(p))
		con("w: " << w);
 *
 * @param s
 * @return pointer to end of string (1 after last char)
 */
inline rad pw(rad s)
{
	while(*s && *s != ' ')
		++s;
	return s;
}

/**
 * parse to end
 * @param s
 * @return pointer to end of string (1 after last char)
 */
inline rad p0(rad s, str& o)
{
	o.clear();
	while(*s)
		o.append(1, *s++);
	return s;
}

/**
 * parse NOT delim
 * @param s
 * @return pointer to end of string (1 after last char)
 */
inline rad pw(rad s, str& o, char delim = ' ')
{
	o.clear();
	while(*s && *s != delim)
		o.append(1, *s++);
	return s;
}

/**
 * Parse word ignoring leading spaces
 * @param s
 * @param i
 * @return
 */
inline rad psw(rad s, str& o, char delim = ' ')
{
	return pw(ps(s), o, delim);
}

/**
 * SubStringOf?
 * @param s
 * @param t
 * @return
 */
inline bool sso(rad sub, rad text)
{
	while(*text && *sub && *text == *sub)
		{ ++text; ++sub; }
	return !(*sub);
}

/**
 *
 * // Find all occurrences of a substring
 * for(rad s = line.c_str(); (s = fnd(s, "45")); ++s)
 * 	con("s: " << s);
 *
 * @param h
 * @param n
 * @return
 */
inline rad fnd(rad h, rad n)
{
	for(; *(h = pt(h, *n)); ++h)
		if(sso(n, h))
			return h;
	return 0;
}

inline rad rsp(rad s) { return s; }
inline rad rsp(rad s, int& a) { return psi(s, a); }
inline rad rsp(rad s, siz& a) { return psz(s, a); }
inline rad rsp(rad s, flt& a) { return psf(s, a); }
inline rad rsp(rad s, str& a) { return psw(s, a); }

template<typename T, typename... Args>
inline rad rsp(rad s, T& t, Args&... args)
{
	return rsp(rsp(s, t), args...);
}

// manipulators

struct mpt { char delim; mpt(char delim):delim(delim){} };
inline rad rsp(rad s, const mpt& m) { return pt(s, m.delim); }

struct mps {};
inline rad rsp(rad s, const mps& ) { return ps(s); }

struct madv { siz d = 0; madv(siz d = 1): d(d){} };
inline rad rsp(rad s, const madv& m) { return adv(s, m.d); }

struct m
{
	static mps ps;
	static madv adv;
};

// usage

// rsp(line.c_str(), i1, f1, mpt('\\'), w1);

}} // sookee::radp

namespace soo { using namespace sookee::radp; }

#endif // LIBSOOKEE_RAD_PARSER_H_