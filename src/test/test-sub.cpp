#include <sookee/types/vec.h>
#include <sookee/str.h>
#include <sookee/bug.h>
#include <sookee/log.h>
#include <sookee/rnd.h>
#include <sookee/sub.h>
#include <sookee/test.h>

using namespace sookee::bug;
using namespace sookee::log;
using namespace sookee::rnd;
using namespace sookee::sub;
using namespace sookee::types;
using namespace sookee::utils;

int main()
{
	con("Start test: sub");
	circular_buffer<char> q(5);

	dump(q);
//	clear(q, '_');
	dump(q);

	for(char i(0); i < 10; ++i)
	{
		char c = 'a' + i;
		std::cout << "in  : '" << c << "'\n";
		std::cout << "out : '" << q.push(c) << "'\n";
		std::cout << "pipe: '" << q << "'\n";
		dump(q);
	}

	std::istringstream iss("a word is a wok a doodle doo");

	sookee::sub::map m;
	m.emplace("word", "XXX");
	m.emplace("wok", "YY");

	std::ostringstream oss;
	map_copy(m, iss, oss);
	std::cout << "result: " << oss.str() << '\n';

	con("End of testing:");
}
