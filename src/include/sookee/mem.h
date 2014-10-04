#ifndef LIBSOOKEE_MEM_H_
#define LIBSOOKEE_MEM_H_
/*
 *  Created on: 23 Sep 2014
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2014 SooKee oasookee@gmail.com                     |
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

#include <vector>

namespace sookee { namespace mem {

// http://stackoverflow.com/questions/11425576/private-constructor-inhibits-use-of-emplace-back-to-avoid-a-move


template<typename Type, typename... Args>
struct is_instantiable
{
	typedef Type type;
	is_instantiable(Args... args) { Type type; }
	static constexpr bool value = true;
};

//template<typename Type, typename... Args>
//struct is_instantiable<Type>
//{
//	typedef Type type;
////	is_instantiable() {}
//	static constexpr bool value = false;
//};

//template<typename T>
//typename std::enable_if<std::is_pod<T>::value, T>::type
//foo(T t) { return t; }
//
//template<typename T>
//typename std::enable_if<!std::is_pod<T>::value, void>::type
//foo(T t) { }

template<typename Type>
class ptr_scope_manager
{
private:
	std::vector<Type> ptrs;

public:
    template <typename T = Type, typename... Args>
    auto create(Args&&... args) -> typename std::enable_if<!is_instantiable<T, Args...>::value, T*>::type
    {
        ptrs.push_back(T{ std::forward<Args>(args)... });
        return &ptrs.back();
    }

    template <typename T = Type, typename... Args>
    auto create(Args&&... args) -> typename std::enable_if<is_instantiable<T, Args...>::value, T*>::type
    {
        ptrs.emplace_back(std::forward<Args>(args)...);
        return &ptrs.back();
    }
};

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}} // sookee::bug

namespace soo { using namespace sookee::mem; }

#endif // LIBSOOKEE_MEM_H_
