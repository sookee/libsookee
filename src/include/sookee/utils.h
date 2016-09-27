/*
 *  Created on: Jul 26, 2014
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_UTILS_H
#define LIBSOOKEE_UTILS_H

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee <oasookee@gmail.com>				   |
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

#include <chrono>

#include <sookee/types/basic.h>
#include <sookee/types/stream.h>
#include <sookee/types/map.h>
#include <sookee/types/str_vec.h>
#include <sookee/ios.h>

namespace sookee { namespace utils {

// seq - generate number sequences

template<typename NumericType>
class seq_iterator
{
public:
	using value_type = NumericType;
	using pointer = value_type*;
	using const_pointer = value_type const*;
	using reference = value_type&;
	using const_reference = value_type const&;

private:
	value_type n {};
	value_type step = 1;

	bool peq(seq_iterator& iter) const { return n >= iter.n; }
	bool neq(seq_iterator& iter) const { return n <= iter.n; }

public:
	seq_iterator(value_type n, value_type step = 1): n(n), step(step) {}
	seq_iterator(seq_iterator&& iter): n(iter.n), step(iter.step) { iter.n = {}; iter.step = 1; }
	seq_iterator(const seq_iterator& iter): n(iter.n), step(iter.step) {}

	bool operator==(seq_iterator& iter) const { return step < 0 ? neq(iter):peq(iter); }
	bool operator!=(seq_iterator& iter) const { return !(*this == iter); }

	reference operator*() { return n; }
	const_reference operator*() const { return n; }

//	pointer operator->() { return ptr; }
//	const_pointer operator->() const { return ptr; }

	seq_iterator& operator++() { n += step; return *this; }
	seq_iterator operator++(int) { seq_iterator iter(*this); n += step; return iter; }
	seq_iterator& operator--() { n -= step; return *this; }
	seq_iterator operator--(int) { seq_iterator iter(*this); n -= step; return iter; }
};

template<typename NumericType>
struct seq
{
	using value_type = NumericType;

	value_type b {};
	value_type e {};
	value_type step;

	seq(value_type b, value_type e, value_type step = 1): b(b), e(e), step(step)
	{
		if(b < e && step <= 0)
			throw std::out_of_range("bad range for seq: "
				+ std::to_string(b) + "->" + std::to_string(e) + " + "
				+ std::to_string(step));
		else if(b > e && step >= 0)
			throw std::out_of_range("bad range for seq: "
				+ std::to_string(b) + "->" + std::to_string(e) + " + "
				+ std::to_string(step));
	}

	seq_iterator<value_type> begin() const { return seq_iterator<value_type>(b, step); }
	seq_iterator<value_type> end() const { return seq_iterator<value_type>(e, step); }
};

using int_seq = seq<int>;
using dbl_seq = seq<double>;

// usage: int_seq(begin, end, step)
//
//	for(auto&& i: int_seq(-10, 20, 1))
//		bug_var(i);

class wait_timer
{
public:
	using clock = std::chrono::steady_clock;
	using duration = clock::duration;

private:
	duration wait;
	clock::time_point time_up;

public:
	wait_timer(duration wait)
	: wait(wait), time_up(clock::now() + wait)
	{
	}

	void pause_thread()
	{
		std::this_thread::sleep_until(time_up);
		time_up += wait;
	}
};

inline std::size_t proc_self_status(std::string const& key = "VmRSS")
{
	std::ifstream ifs("/proc/self/status");

	std::string k, v;

	std::size_t m;
	while(std::getline(std::getline(ifs, k, ':') >> std::ws, v))
		if(k == key)
			if(std::istringstream(v) >> m)
				return m * 1024;

	return -1;
}

class Resolver
{
	str_vec seen;
	str_vec resolved;
	std::map<str, str_vec> deps;

	void resolve(str const& d)
	{
		bug("dep_resolve: " << d);
		seen.push_back(d);
		for(auto const& nd: deps[d])
		{
			if(std::find(resolved.begin(), resolved.end(), nd) != resolved.end())
				continue;
			else if(std::find(seen.begin(), seen.end(), nd) == seen.end())
				resolve(nd);
			else
			{
				log("E: plugin " << d << " has a circular dependency with " << nd);
				continue;
			}
		}
		bug("     adding: " << d);
		resolved.push_back(d);
	}

public:

	// clear internal state without
	// reducing memory allocation
	void clear()
	{
		seen.clear();
		resolved.clear();
		deps.clear();
	}

	// clear and reduce memory consumption
	// to minimum
	void clean(unsigned reserve_guess = 20)
	{
		str_vec().swap(seen);
		seen.reserve(reserve_guess);
		str_vec().swap(resolved);
		resolved.reserve(reserve_guess);
		std::map<str, str_vec>().swap(deps);
	}

	void add(str const& a)
	{
		deps[a];
		seen.reserve(deps.size());
	}

	void add(str const& a, str const& b)
	{
		deps[a].push_back(b);
		seen.reserve(deps.size());
	}

	str_vec const& resolve()
	{
		for(auto const& d: deps)
			if(std::find(resolved.begin(), resolved.end(), d.first) == resolved.end())
				resolve(d.first);
		return resolved;
	}
};

}} // ::sookee::utils

namespace soo { using namespace sookee::utils; }

#endif // LIBSOOKEE_UTILS_H
