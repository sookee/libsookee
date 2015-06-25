/*-----------------------------------------------------------------*
 | File    : sub.cpp
 | Project : utils
 | Created : 12 Oct 2010
 | Author  : grafterman@googlemail.com
 *-----------------------------------------------------------------*/
#include <sookee/sub.h>

#include <map>
#include <string>
#include <algorithm>

//------------------------------------------------------------------
namespace sookee { namespace sub {
//------------------------------------------------------------------

typedef map::iterator map_iter;
typedef map::const_iterator map_citer;

typedef circular::iterator que_iter;
typedef circular::const_iterator que_citer;

std::ostream& operator<<(std::ostream& os, const circular& q)
{
	for(que_citer i(q.begin()); i != q.end(); ++i)
	{
		os << *i;
	}
	return os;
}

void map_copy(map& m, std::istream& is, std::ostream& os)
{
	//std::cout << "THIS\n";
	if(m.empty()) // straight copy
	{
		size_t len;
		char buf[2048];
		while((len = is.readsome(buf, 2048)) > 0) os.write(buf, len);
		return;
	}

	// replace copy

	char c; // general purpose
	circular pipe(m.begin()->first.size());

	for(size_t p = 0; p < pipe.capacity() && is.get(c); ++p)
		pipe.push(c);

	map_citer i;
	map_citer end = m.end();
	while(!pipe.empty())
	{
//		pipe.dump();
		for(i = m.begin(); i != end && !pipe.contains(i->first); ++i);
		if(i != end)
		{
			os.write(i->second.c_str(), i->second.length());
			for(size_t p = 0; p < i->first.length(); ++p)
			{
				pipe.pull();
				if(is.get(c))
					pipe.push(c);
			}
		}
		else
		{
			if(pipe.pull(c))
				os.put(c);
			if(is.get(c))
				pipe.push(c);
		}
	}
}

std::string map_replace(map& m, const std::string& s)
{
	using size_type = std::string::size_type;
	using map_citer = map::const_iterator;

	if(m.empty())
		return s;

	size_type idx = 0;
	size_type max = s.size();

	std::string r;
	r.reserve(max * 2);

	char c; // general purpose
	circular pipe(m.begin()->first.size());

	for(; idx < pipe.capacity() && idx < max; ++idx)
		pipe.push(s[idx]);

	map_citer i;
	map_citer end = m.end();
	while(!pipe.empty())
	{
		for(i = m.begin(); i != end && !pipe.contains(i->first); ++i);
		if(i != end)
		{
			r += i->second;
			for(size_type p = 0; p < i->first.length(); ++p)
			{
				pipe.pull();
				if(idx < max)
					pipe.push(s[idx++]);
			}
		}
		else
		{
			if(pipe.pull(c))
				r += c;
			if(idx < max)
				pipe.push(s[idx++]);
		}
	}
	return r;
}

void map_copy_reset(map& m, std::istream& is, std::ostream& os)
{
	is.clear();
	is.seekg(0);
	map_copy(m, is, os);
}

void dump(circular_buffer<char>& c)
{
	std::cout << "i: ";
	circular_buffer<char>::iterator i = c.begin();
	for(;i != c.end(); ++i)
	{
		std::cout << *i;
	}
	std::cout << '\n';
	std::cout << "a: ";
	for(size_t i(0); i < c.size(); ++i)
	{
		std::cout << c[i];
	}
	std::cout << '\n';
}

//------------------------------------------------------------------
}} // sookee::sub
//------------------------------------------------------------------
