#ifndef LIBSOOKEE_SUB_H
#define LIBSOOKEE_SUB_H

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee oasookee@gmail.com                     |
'------------------------------------------------------------------'

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

http://www.gnu.org/licenses/gpl-2.0.html

'-----------------------------------------------------------------*/

#include <map>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace sookee { namespace sub {

struct is_longer_than
{
	bool operator()(const std::string& a, const std::string& b) { return a.size() >= b.size(); }
};

using map = std::multimap<std::string, std::string, is_longer_than>;

struct is_longer_than_lex
{
	bool operator()(const std::string& a, const std::string& b)
	{
		if(a.size() > b.size()) return true;
		if(a.size() < b.size()) return false;
		return a < b;
	}
};

template<typename T>
class circular_buffer
{
	friend class iterator;
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef size_t size_type;

private:
	const size_type len;
	T* buf;
	size_type top;
	size_type bot;
	bool full;
public:
	circular_buffer(size_type size)
	: len(size), buf(new T[len]), top(0), bot(0), full(false) {}//{ std::fill_n(buf, size, '_'); }
	~circular_buffer()
	{
		delete[] buf;
	}

	size_t empty() const { return bot == top && !full; }

	reference operator[](size_type i) const { return buf[(bot + i) % len]; }
	size_type capacity() const { return len; }
	size_type size() const
	{
		if(empty()) return 0;
		if(top > bot) return top - bot;
		if(top < bot) return len - top + bot;
		return 0;
	}

	bool push(const T& c)
	{
		if(full)
			return false;
		buf[top] = c;
		top = top == len - 1 ? 0 : top + 1;
		if(top == bot)
			full = true;
		return true;
	}

	bool pull()
	{
		T c;
		return pull(c);
	}

	bool pull(T& c)
	{
		if(empty())
			return false;
		c = buf[bot];
//		buf[top] = '_'; // TODO: remove this
		bot = bot == len - 1 ? 0 : bot + 1;
		full = false;
		return true;
	}

	void ignore(size_t len = 1)
	{
		for(size_t i = 0; i < len; ++i)
			pull();
	}

	void dump(std::ostream& os = std::cout) const
	{
		os << "dump: ";
		for(size_t i = 0; i < len; ++i)
		{
			if(i == bot) { os << "["; }
			os << buf[i];
			if(i == top) { os << "]"; }
		}
		os << std::endl;
	}

	bool contains(const std::string& s) const
	{
		if(s.size() > len)
			return false;

		for(size_t p = 0; p < s.size(); ++p)
			if(s[p] != (*this)[p])
				return false;

		return true;
	}

	size_type read(std::istream& is, size_type len = size())
	{
		size_type i = 0;
		while(is && i < len && push(is.peek()))
			{ is.ignore(); ++i; }
		return i;
	}

	class iterator;
	//typedef typename circular_buffer<const T>::iterator const_iterator;

	class iterator
	: public std::input_iterator_tag
	{
	private:
		static T zero;
		circular_buffer* c;
		size_type pos;

	public:
		iterator(circular_buffer* c = 0): c(c), pos(0) {}
		iterator(const iterator& i): c(i.c), pos(i.pos) {}
		~iterator() {}

		iterator& operator=(const iterator& i) { c = i.c; pos = i.pos; return *this; }

		reference operator*() { return c ? (*c)[pos] : zero; }
		pointer operator->() { return c ? &((*c)[pos]) : &zero; }
		iterator& operator++()
		{
			if(c && ++pos == c->len) { c = 0; pos = 0; }
			return *this;
		}
		iterator& operator++(int) { return operator++(); }
		bool operator==(const iterator& i) const
		{
			return c == i.c && pos == i.pos;
		}
		bool operator!=(const iterator& i) const { return !operator==(i); }
	};

	class const_iterator
	: public std::input_iterator_tag
	{
	private:
		static T zero;
		const circular_buffer* c;
		size_type pos;

	public:
		const_iterator(const circular_buffer* c = 0): c(c), pos(0) {}
		const_iterator(const const_iterator& i): c(i.c), pos(i.pos) {}
		~const_iterator() {}

		const_iterator& operator=(const const_iterator& i) const { c = i.c; pos = i.pos; return *this; }

		const_reference operator*() const { return c ? (*c)[pos] : zero; }
		const_pointer operator->() const { return c ? &((*c)[pos]) : &zero; }
		const_iterator& operator++()
		{
			if(c && ++pos == c->len) { c = 0; pos = 0; }
			return *this;
		}
		const_iterator& operator++(int) { return operator++(); }
		bool operator==(const const_iterator& i) const
		{
			return c == i.c && pos == i.pos;
		}
		bool operator!=(const const_iterator& i) const { return !operator==(i); }
	};

	iterator begin() { return iterator(this); }
	const_iterator begin() const { return const_iterator(this); }
	iterator end() { return iterator(); }
	const_iterator end() const { return const_iterator(); }
};


template<typename T>
T circular_buffer<T>::iterator::zero = T();
template<typename T>
T circular_buffer<T>::const_iterator::zero = T();

typedef circular_buffer<char> circular;

std::ostream& operator<<(std::ostream& os, const circular& q);

void map_copy(map& m, std::istream& is, std::ostream& os);

std::string map_replace(map& m, const std::string& str);

void map_copy_reset(map& m, std::istream& is, std::ostream& os);

void dump(circular_buffer<char>& c);

}} // sookee::sub

#endif // LIBSOOKEE_SUB_H
