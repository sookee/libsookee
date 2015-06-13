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

//template<typename Numeric = double, size_t SIZE = 10>
//class MovingAverage
//{
//	std::deque<Numeric> data;
//
//	siz num = 0;
//	Numeric sum = {}; // cache sum(data)
////	Numeric ave = {};
//
//public:
//	MovingAverage() {}
//
//	void add(Numeric num)
//	{
//		data.push_back(num);
//		sum += data.back();
//
//		if(data.size() > SIZE)
//		{
//			sum -= data.front();
//			data.pop_front();
//		}
//	}
//	Numeric average() const { return sum / data.size(); }
//};

template<typename NumericType = int, typename ResultType = float>
class MovingAverage
{
	using size_type = typename std::vector<NumericType>::size_type;

	std::vector<NumericType> v;
	size_type pos = 0;
	NumericType sum = 0;

public:
	MovingAverage(size_type size)
	: v(size) { if(!size) throw std::range_error("can't be zero"); }

	void add(NumericType n)
	{
		sum = sum + n - v[pos];
		v[pos] = n;

		if(++pos == v.size())
			pos = 0;
	}

	ResultType get() const
	{
		return ResultType(sum) / v.size();
	}

	ResultType next(NumericType n)
	{
		add(n);
		return get();
	}
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

template<typename Type, std::size_t X, std::size_t Y>
class matrix2d
{
	typedef std::size_t size_type;
	typedef Type ((&TypeRef)[X])[Y];
	typedef const Type ((&ConstTypeRef)[X])[Y];

	Type array[X][Y];

public:
	matrix2d(): array{0}
	{
	}

	operator TypeRef() { return array; }
	operator ConstTypeRef() const { return array; }

	constexpr size_type size_x() const { return X; }
	constexpr size_type size_y() const { return Y; }

	void dump() const
	{
		for(unsigned y = 0; y < Y; ++y)
		{
			for(unsigned x = 0; x < X; ++x)
			{
				if(array[x][y] < 10)
					std::cout << ' ';
				std::cout << array[x][y] << ' ';
			}
			std::cout << '\n';
		}
	}
};

//template<typename Type>
//void minmax(Type& a, Type& b)
//{
//	if(b < a)
//		std::swap(a, b);
//}

}} // sookee::math

namespace soo { using namespace sookee::math; }

#endif // LIBSOOKEE_MATH_H_
