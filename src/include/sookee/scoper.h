/*
 *  Created on: Mar 5, 2015
 *	  Author: SooKee <oasookee@gmail.com>
 */

#ifndef LIBSOOKEE_SCOPER_H_
#define LIBSOOKEE_SCOPER_H_

/*-----------------------------------------------------------------.
| Copyright (C) 2015 SooKee <oasookee@gmail.com>				   |
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

namespace sookee { namespace utils {

// deprecated, use RAII below

template<typename Type, typename Functor>
struct scoper
{
	Type& obj;
	Functor func;
	scoper(Type& obj): obj(obj) {}
	~scoper() { func(obj); }
};

struct null_functor
{
	void operator()() {}
};

template<typename Type>
struct clear_functor
{
	void operator()(Type& obj) { obj.clear(); }
};

template<typename Type>
using clear_scoper = scoper<Type, clear_functor<Type>>;

template<typename Type>
struct global_close_functor
{
	void operator()(Type& obj) { ::close(obj); }
};

template<typename Type>
using global_close_scoper = scoper<Type, global_close_functor<Type>>;

template<typename Type>
struct free_functor
{
	void operator()(Type& obj) { free(obj); }
};

template<typename Type>
using free_scoper = scoper<Type, free_functor<Type>>;

template<typename Type>
auto make_clear_scoper(Type& obj) { return clear_scoper<Type>(obj); }

template<typename Type>
auto make_free_scoper(Type& obj) { return free_scoper<Type>(obj); }

template<typename Type>
auto make_global_close_scoper(Type& obj) { return global_close_scoper<Type>(obj); }

// RAII ==================================================

template<typename DataType>
struct raii
{
	DataType* d = nullptr;
	std::function<void(DataType)> f;
	friend void swap(raii& a, raii& b) { std::swap(a.d, b.d); std::swap(a.f, b.f); }

	raii(): d(nullptr) {}
	raii(DataType& d, std::function<void(DataType)> f): d(&d), f(f) {}
	raii(const raii&) = delete;
	raii(raii&& r): raii() { swap(*this, r); }
	~raii() { if(d) f(*d); }

	raii& operator=(const raii&) = delete;
	raii& operator=(raii&& r) { swap(*this, r); return *this; }

	void reset(DataType* d) { if(this->d) f(*this->d); this->d = d; }
	DataType* release() { DataType* d = this->d; this->d = nullptr; return d; }
};

auto lambda_unistd_close = [](int fd)
{
	::close(fd);
};

auto lambda_stdlib_free = [](void* vp)
{
	::free(vp);
};

auto lambda_object_clear = [](auto& obj)
{
	obj.clear();
};

auto make_unistd_closer(int fd)->raii<int>
{
	return {fd, lambda_unistd_close};
}

auto make_logged_unistd_closer(int fd)->raii<int>
{
	auto lambda = [](int fd)
	{
		if(::close(fd) == -1)
			log("E: " << std::strerror(errno));
	};
	return {fd, lambda};
}

auto make_stdlib_freer(void* vp)->raii<void*>
{
	return {vp, lambda_stdlib_free};
}

template<typename Object>
auto make_object_clearer(Object& o)->raii<Object>
{
	return {o, lambda_object_clear};
}

}} // ::sookee::utils

namespace soo { using namespace sookee::utils; }

#endif // LIBSOOKEE_SCOPER_H_
