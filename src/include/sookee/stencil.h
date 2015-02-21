#ifndef LIBSOOKEE_TEMPLATE_H_
#define LIBSOOKEE_TEMPLATE_H_
/*
 *  Created on: 28 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/ios.h>
#include <sookee/types/basic.h>
#include <sookee/types/stream.h>
#include <sookee/types/siz_vec.h>
#include <sookee/types/str_vec.h>
#include <sookee/str.h>
#include <sookee/radp.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <string>

namespace sookee { namespace utils {

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::ios;
using namespace sookee::types;
using namespace sookee::utils;

/**
 * Stencils are string templates with replaceable sections.
 *
 *
 */
class arg_stencil
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
	arg_stencil() {}
	arg_stencil(arg_stencil&& s): size(s.size), pieces(s.pieces), params(s.params)
	, args(s.args), d1(s.d1), d2(s.d2) {}
	arg_stencil(const arg_stencil& s): size(s.size), pieces(s.pieces), params(s.params)
	, args(s.args), d1(s.d1), d2(s.d2) {}

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
	arg_stencil(const str& d1, const str& d2): d1(d1), d2(d2) {}

	arg_stencil(const str& text, const str& d1 = "${", const str& d2 = "}"): d1(d1), d2(d2) { compile(text); }

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

			if(sookee::radp::psz(param_spec.c_str(), param) == param_spec.c_str())
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

class map_stencil
{
	// order: piece, var, piece, var, piece
	str_vec pieces;
	str_vec vars;

	const str d1 = "${";
	const str d2 = "}";

public:
	using dict = str_map;

	/**
	 * Create an empty stencil
	 */
	map_stencil() {}
	map_stencil(map_stencil&& s): pieces(s.pieces), vars(s.vars)
	, d1(s.d1), d2(s.d2) {}
	map_stencil(const map_stencil& s): pieces(s.pieces), vars(s.vars)
	, d1(s.d1), d2(s.d2) {}

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
	map_stencil(const str& d1, const str& d2): d1(d1), d2(d2) {}

	map_stencil(const str& text, const str& d1 = "${", const str& d2 = "}")
	: d1(d1), d2(d2) { compile(text); }

	// ${var}
	void clear()
	{
		pieces.clear();
		vars.clear();
	}

	bool compile_file(const str& filename)
	{
		return compile((sss() << sifs(filename).rdbuf()).str());
	}

	bool compile(const str& text)
	{
		clear();
		auto beg = text.begin();
		auto pos = beg;
		auto fin = text.end();
		auto end = pos;

		while(pos != fin)
		{
			end = std::search(pos, fin, d1.begin(), d1.end());

//			if(end != beg)
				pieces.emplace_back(pos, end);

			if(end == fin)
				break;

			pos = std::next(end, d1.size());

			if((end = std::search(pos, fin, d2.begin(), d2.end())) == fin)
			{
				log("ERROR: expected " << d2);
				return false;
			}

			vars.emplace_back(pos, end);

			pos = std::next(end, d2.size());

//			if(pos == fin)
//				pieces.emplace_back("");
		}

		return true;
	}

	void dump() const
	{
		auto p = pieces.begin();
		auto v = vars.begin();

		while(p != pieces.end())
		{
			con("p: " << *p++);
			if(v != vars.end())
				con("v: " << *v++);
		}

		while(v != vars.end())
		{
			con("v: " << *v++);
			if(p != pieces.end())
				con("p: " << *p++);
		}
	}

	str create(const dict& d) const
	{
		str s;
		auto p = pieces.begin();
		auto v = vars.begin();

		while(p != pieces.end())
		{
			s.append(*p++);
			if(v != vars.end())
			{
				if(!d.count(*v))
					log("ERROR: not in dict" << *v++);
				else
					s.append(d.at(*v++));
			}
		}

		while(v != vars.end())
		{
			if(!d.count(*v))
				log("ERROR: not in dict" << *v++);
			else
				s.append(d.at(*v++));
			if(p != pieces.end())
				s.append(*p++);
		}

		return s;
	}
};

}} // ::sookee::utils

#endif /* LIBSOOKEE_TEMPLATE_H_ */
