#ifndef LIBSOOKEE_TEMPLATE_H_
#define LIBSOOKEE_TEMPLATE_H_
/*
 *  Created on: 28 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/types/basic.h>
#include <sookee/types/siz_vec.h>
#include <sookee/types/str_vec.h>
#include <sookee/str.h>
#include <sookee/radp.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <string>

namespace sookee { namespace string {

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::types;
using namespace sookee::string;

/**
 * Stencils are string templates with replaceable sections.
 *
 *
 */
class stencil
{
private:
	siz size = 0;
	str_vec pieces;
	siz_vec params;

	str_vec args; // prepared args

	const str d1 = "${";
	const str d2 = "}";

	str get_val(const str_map& m, const str& key) const
	{
		if(m.count(key))
			return m.at(key);
		return "";
	}

	str get_arg(siz) const
	{
		return "";
	}

	str get_arg(siz idx, const char* arg) const
	{
		if(!idx)
			return arg;
		log("ERROR: idx out of range");
		return "";
	}

	str get_arg(siz idx, const str& arg) const
	{
		if(!idx)
			return arg;
		log("ERROR: idx out of range");
		return "";
	}

	template<typename Arg>
	str get_arg(siz idx, Arg arg) const
	{
		if(!idx)
			return std::to_string(arg);
		log("ERROR: idx out of range");
		return "";
	}

	template<typename... Args>
	str get_arg(siz idx, const char* arg, Args... args) const
	{
		if(!idx)
			return arg;
		else
			return get_arg(idx - 1, args...);
	}

	template<typename... Args>
	str get_arg(siz idx, const str& arg, Args... args) const
	{
		if(!idx)
			return arg;
		else
			return get_arg(idx - 1, args...);
	}

	template<typename Arg, typename... Args>
	str get_arg(siz idx, Arg arg, Args... args) const
	{
		if(!idx)
			return std::to_string(arg);
		else
			return get_arg(idx - 1, args...);
	}

public:
	/**
	 * Create an empty stencil
	 */
	stencil() {}

	/**
	 * Create a stencil whose parameter markers are delimited
	 * as beginning with @param d1 and ending with @param d2.
	 *
	 * e.g. ${n} (where n is the parameter number
	 *
	 * d1 = "${", d2 = "}"
	 *
	 * @param d1 the start delimiter of embedded parameters
	 * @param d2 the end delimiter of embedded parameters
	 */
	stencil(const str& d1, const str& d2): d1(d1), d2(d2) {}

	/**
	 * Clear the stencil of all data,
	 */
	void clear()
	{
		size = 0;
		pieces.clear();
		params.clear();
		args.clear();
	}

	/**
	 * Add a constant argument for this stencil
	 *
	 * These arguments are matched first (in order they are added)
	 * before arguments supplied to the create() function.
	 *
	 * @param arg The argument to add to the list of constant args.
	 */
	void add(const str& arg)
	{
		args.push_back(arg);
	}

	template<typename Arg>
	void add(const Arg& arg)
	{
		add(std::to_string(arg));
	}

	void dump() const
	{
		for(const auto& v: params)
			con("param: " << v);
		for(const auto& p: pieces)
			con("piece: " << p);
		for(const auto& a: args)
			con("args : " << a);
	}

	/**
	 * Create a string from this template mathing first the internal
	 * args added using the add() function and then the args supplied
	 * as parameters to this function.
	 *
	 * @param args
	 * @return
	 */
	template<typename... Args>
	str create(Args... args) const
	{
		str s;
		// TODO: figure best way to guess capacity
		//s.reserve(size + params.size() * 64);
//		siz c = s.capacity();

		siz_vec_citer v = params.cbegin();
		siz_vec_citer vend = params.cend();
		str_vec_citer p = pieces.cbegin();
		str_vec_citer pend = pieces.cend();

		siz arg;
		while(v != vend && p != pend)
		{
			if(p != pend)
				s.append(*p++);

			if(v != vend)
			{
				arg = (*v++) - 1;
//				bug_var(arg);
//				bug_var(this->args.size());
				if(arg < this->args.size())
					s.append(this->args[arg]);
				else
					s.append(get_arg(arg - this->args.size(), args...));
			}
		}

		while(p != pend)
			s.append(*p++);

		while(v != vend)
		{
			arg = (*v++) - 1;
			if(arg < this->args.size())
				s.append(this->args[arg]);
			else
				s.append(get_arg(arg - this->args.size(), args...));
		}

//		if(s.capacity() > c)
//			log("WARN: capacity (" << c << ") exceeded by " << (s.capacity() - c));
		return s;
	}

	/**
	 * Prepare the stencil's data.
	 * @param tmp
	 * @return
	 */
	bool compile(const str& tmp)
	{
		clear();

		size = tmp.size();

		siz cur, end;
		siz pos = cur = 0;

		while(pos != str::npos)
		{
			if((pos = tmp.find(d1, cur)) == str::npos)
			{
				pieces.emplace_back(tmp.substr(cur));
				break;
			}

			if((end = tmp.find(d2, pos)) == str::npos)
			{
				log("ERROR: expected end of variable: " << pos);
				return false;
			}

			siz param;
			str param_spec = tmp.substr(pos + d1.size(), end - (pos  + d1.size()));

			if(soo::psz(param_spec.c_str(), param) == param_spec.c_str())
			{
				log("ERROR: parsing <?soo ?> parameter value: " << pos);
				return false;
			}

			pieces.emplace_back(tmp.substr(cur, pos - cur));
			params.push_back(param);
			cur = end + d2.size();
			pos = cur;
		}

		return true;
	}
};

}} // ::sookee::string

#endif /* LIBSOOKEE_TEMPLATE_H_ */
