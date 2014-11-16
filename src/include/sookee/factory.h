#ifndef _LIBSOOKEE_FACTORY_H_
#define _LIBSOOKEE_FACTORY_H_
/*
 *  Created on: 21 Sep 2014
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

namespace sookee { namespace factory {

template<typename BaseType>
struct BaseTypeCreator
{
	virtual ~BaseTypeCreator() {}
	virtual BaseType* create() const = 0;
};

template<typename BaseType, typename CreatedType>
struct DerivedTypeCreator
: public BaseTypeCreator<BaseType>
{
	BaseType* create() const { return new CreatedType; }
};

class Message
{
};

class Message1
: public Message
{
};

class Message2
: public Message
{
};

class Message3
: public Message
{
};

template<typename MessageType>
struct MessageCreator
: public DerivedTypeCreator<Message, MessageType>
{

};

const BaseTypeCreator<Message>* creators[] =
{
	new MessageCreator<Message1>()
	, new MessageCreator<Message2>()
	, new MessageCreator<Message3>()
};

//void example()
//{
//	for(size_t id = 0; id < sizeof(creators)/sizeof(BaseTypeCreator<Message>*); ++id)
//		creators[id]->create()->print();
//}

}} // sookee::factory

// namespace soo { using namespace sookee::bug; }

#endif // _LIBSOOKEE_BUG_H_
