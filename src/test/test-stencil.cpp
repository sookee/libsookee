/*
 *  Created on: 29 Jul 2014
 *      Author: SooKee <oasookee@gmail.com>
 */

#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/stencil.h>
#include <sookee/test.h>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::test;
using namespace sookee::utils;

template<template <typename...> class MapType>
class stencil_nt
{
public:
	using map_type = MapType<std::string, std::string>;
	using size_type = std::string::size_type;

private:
	using str_vec = std::vector<std::string>;

//	std::string d1 = "{@";
	std::string d1 = "${";
	std::string d2 = "}";

	bool sub = false; // begin with sub?
	str_vec items;

public:
	void compile(std::string const& doc)
	{
		items.clear();
		size_type pos = 0;
		size_type end = 0;

		sub = false;
		while((end = doc.find(d1, pos)) != std::string::npos)
		{
			if(!end)
				sub = true;
			else
				items.push_back(doc.substr(pos, end - pos));

			pos = end + d1.size();
			if((end = doc.find(d2, pos)) == std::string::npos)
				throw std::runtime_error("no substitution end marker found");

			items.push_back(doc.substr(pos, end - pos));
			pos = end + d2.size();
		}

		if(pos < doc.size())
			items.push_back(doc.substr(pos));
	}

	void dump(std::ostream& os)
	{
		bool sub = !this->sub;
		for(auto const& item: items)
			os << '\t' << '"' << item << '"' << ": " << std::boolalpha << (sub = !sub) << '\n';
	}

	bool sub_tog;
	bool eofbit = false;

	size_type count;
	size_type sent = 0;
	size_type item_idx = 0;

	void open()
	{
		sub_tog = sub;
		eofbit = false;
		item_idx = 0;
		sent = 0;
	}

	size_type size(const map_type& m) const
	{
		bool sub = this->sub;
		size_type sz = 0;
		for(auto const& s: items)
		{
			decltype(m.cend()) found;
			size_type c = s.size();
			if(!(sub = !sub))
				if((found = m.find(s)) != m.end())
					c = found->second.size();
			sz += c;
		}
		return sz;
	}

	std::string create(const map_type& m)
	{
		open();
		char buf[1024];
		std::string text;
//		text.reserve(size(m));
//		text.reserve(1024);
		while(read(buf, sizeof(buf), m) && gcount())
			text.append(buf, gcount());
		return text;
	}

	size_type gcount() const { return count; }

	template<size_type N>
	bool read(std::array<char, N>& buf, const map_type& m)
	{
		return read(buf.data(), buf.size(), m);
	}

	bool read(std::vector<char>& buf, const map_type& m)
	{
		return read(buf.data(), buf.size(), m);
	}

	bool read(char* buf, size_type len, const map_type& m)
	{
		if(!(item_idx < items.size()))
		{
			eofbit = true;
			return false;
		}

		std::string const* s;
		decltype(m.cend()) found;

		do
		{
			s = &items[item_idx];
//			bug_var(s);
			if(sub_tog)
				if((found = m.find(*s)) != m.end())
					s = &found->second;
//			bug_var(s);

			if((count = std::min(len, s->size() - sent)))
				break;

			sub_tog = !sub_tog;
			++item_idx;
			sent = 0;
		}
		while(item_idx < items.size());

		if(!(item_idx < items.size()))
		{
			eofbit = true;
			return false;
		}

//		bug_var(count);

		std::copy(s->data() + sent, s->data() + sent + count, buf);

		sent += count;

		if(sent == s->size())
		{
			sub_tog = !sub_tog;
			++item_idx;
			sent = 0;
		}

		return true;
	}
};

const str_vec arg_tests =
{
	""
	, "${1}"
	, "${ 1}"
	, "${1 }"
	, "${ 1 }"
	, "${1}${2}"
	, "${1} ${2}"
	, "X${  1  }"
	, "${1}X"
	, "X${1}${2}"
	, "${1}X${2}"
	, "${1}${2}X"
	, "X${1}X${2}"
	, "${1}X${2}X"
};

const str_vec map_tests =
{
	"start"
	, "${v1}"
	, "start${v1}"
	, "${v1}end"
	, "start${v1}end"
	, "start${v1}mid1${v2}mid2${v1}end"
	, "start${v1}mid1${v2}mid2${v1}end"
};

const str timer_test = R"(
0123456789 ${1} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${2} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${3} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${4} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${5} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${6} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${7} ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789 ${8} ABCDEFGHIJKLMNOPQRSTUVWXYZ
)";

int main()
{
	for(const auto& tmp: arg_tests)
	{
		arg_stencil tplt;
		tplt.compile(tmp);
//		tplt.dump();
		con("-----------------------------------------------");
		con("tmp: " << tmp);
		con("   : " << tplt.create("ARG1", "LONGARG2"));
	}

	map_stencil::dict d {{"v1", "s1"}, {"v2", "s2"}};

	map_stencil s;

	for(auto&& t: map_tests)
	{
		s.compile(t);
//		s.dump();
		con(s.create(d));
		con("");
	}

	const uns repeats = 1000000;

	Timer timer;

	arg_stencil as;
	map_stencil ms;
	stencil_nt<std::map>  nt;
	str ar, mr;

	const str_vec subs = {"one", "two", "three", "four", "five", "six", "seven", "eight"};

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		as.compile(timer_test);
		ar = as.create(subs[0], subs[1], subs[2], subs[3], subs[4], subs[5], subs[6], subs[7]);
	}
	timer.stop();

	con(ar);
	con(timer);

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		ms.compile(timer_test);
		mr = ms.create({{"1", subs[0]}, {"2", subs[1]}, {"3", subs[2]}, {"4", subs[3]}
			, {"5", subs[4]}, {"6", subs[5]}, {"7", subs[6]}, {"8", subs[7]}});
	}
	timer.stop();

	con(mr);
	con(timer);

	map_stencil::dict d2 {{"1", subs[0]}, {"2", subs[1]}, {"3", subs[2]}, {"4", subs[3]}
	, {"5", subs[4]}, {"6", subs[5]}, {"7", subs[6]}, {"8", subs[7]}};

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
		ms.compile(timer_test);
		mr = ms.create(d2);
	}
	timer.stop();

	con(mr);
	con(timer);

	decltype(nt)::map_type d3 {{"1", subs[0]}, {"2", subs[1]}, {"3", subs[2]}, {"4", subs[3]}
	, {"5", subs[4]}, {"6", subs[5]}, {"7", subs[6]}, {"8", subs[7]}};

	timer.start();
	for(uns i = 0; i < repeats; ++i)
	{
//		nt.compile(timer_test);
//		mr = nt.create(d3);
	}
	timer.stop();

	con(mr);
	con(timer);
}







