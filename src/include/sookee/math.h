#ifndef LIBSOOKEE_MATH_H_
#define LIBSOOKEE_MATH_H_
/*
 *  Created on: 08 Oct 2014
 *	  Author: oasookee@gmail.com
 */

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

template<typename Numeric = double, size_t SIZE = 10>
class MovingAverage
{
	std::deque<Numeric> data;

	siz num = 0;
	Numeric sum = {}; // cache sum(data)
	Numeric ave = {};

public:
	MovingAverage() {}

	void add(double num)
	{
		data.push_back(num);
		sum += data.back();

		if(data.size() > SIZE)
		{
			sum -= data.front();
			data.pop_front();
		}

		ave = sum / data.size();
	}
	Numeric average() const { return ave; }
};

constexpr int cfct(int x)
{
	return x == 1 ? x : x * cfct(x - 1);
}

constexpr float cpow(float x, int p)
{
	return p == 1 ? x : x * cpow(x, p - 1);
}

constexpr float csin(float x)
{
	return x - (cpow(x, 3) / cfct(3)) + (cpow(x, 5) / cfct(5)) - (cpow(x, 7) / cfct(7));
}

}} // sookee::math

// namespace soo { using namespace sookee::math; }

#endif // LIBSOOKEE_MATH_H_
