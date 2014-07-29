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
#include <string>

namespace sookee { namespace string {

//using namespace sookee::radp;
using namespace sookee::types;
using namespace sookee::string;

class stencil
{
private:
	siz size = 0;
	str_vec pieces;
	siz_vec params;

	const str d1 = "${";
	const str d2 = "}";

	str get_val(const str_map& m, const str& key) const
	{
		if(m.count(key))
			return m.at(key);
		return "";
	}

	str get_arg(siz idx, const char* arg)
	{
		if(!idx)
			return arg;
		log("ERROR: idx out of range");
		return "";
	}

	template<typename Arg>
	str get_arg(siz idx, Arg arg)
	{
		if(!idx)
			return std::to_string(arg);
		log("ERROR: idx out of range");
		return "";
	}

	template<typename... Args>
	str get_arg(siz idx, const char* arg, Args... args)
	{
		if(!idx)
			return arg;
		else
			return get_arg(idx - 1, args...);
	}

	template<typename Arg, typename... Args>
	str get_arg(siz idx, Arg arg, Args... args)
	{
		if(!idx)
			return std::to_string(arg);
		else
			return get_arg(idx - 1, args...);
	}

public:
	stencil() {}
	stencil(const str& d1, const str& d2): d1(d1), d2(d2) {}

	void clear()
	{
		size = 0;
		pieces.clear();
		params.clear();
	}

	void dump()
	{
		for(const auto& v: params)
			con("param: " << v);
		for(const auto& p: pieces)
			con("piece: " << p);
	}

	template<typename... Args>
	str create(Args... args)
	{
		str s;
		// TODO: figure best way to guess capacity
		//s.reserve(size + params.size() * 64);
//		siz c = s.capacity();

		siz_vec_citer v = params.cbegin();
		siz_vec_citer vend = params.cend();
		str_vec_citer p = pieces.cbegin();
		str_vec_citer pend = pieces.cend();

		while(v != vend && p != pend)
		{
			if(p != pend)
				s.append(*p++);

			if(v != vend)
				s.append(get_arg((*v++) - 1, args...));
		}

		while(p != pend)
			s.append(*p++);

		while(v != vend)
			s.append(get_arg((*v++) - 1, args...));

//		if(s.capacity() > c)
//			log("WARN: capacity (" << c << ") exceeded by " << (s.capacity() - c));
		return s;
	}

	bool compile(const str& tmp)
	{
		clear();
		size = tmp.size();
		siz cur, end;
		siz pos = cur = 0;
		while(pos != str::npos)
		{
//			bug("-------------------------");
			if((pos = tmp.find(d1, cur)) == str::npos)
			{
				pieces.emplace_back(tmp.substr(cur));
				break;
			}

//			bug_var(pos);

			// "Some ${1} text to rep${2s}"

			if((end = tmp.find(d2, pos)) == str::npos)
			{
				log("ERROR: expected end of variable: " << pos);
				return false;
			}

//			bug_var(end);

			siz param;
			str param_spec = tmp.substr(pos + d1.size(), end - (pos  + d1.size()));
//			bug_var(param_spec);
			if(soo::psz(param_spec.c_str(), param) == param_spec.c_str())
			{
				log("ERROR: parsing <?soo ?> parameter value: " << pos);
				return false;
			}
//			bug_var(param);

			pieces.emplace_back(tmp.substr(cur, pos - cur));
			//vars.emplace_back(tmp.substr(pos + 2, end - pos - 2));
			params.push_back(param);
			cur = end + d2.size();
			pos = cur;
		}

		return true;
	}
};

}} // ::sookee::string

#endif /* LIBSOOKEE_TEMPLATE_H_ */
