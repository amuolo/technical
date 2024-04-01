#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE InputOutput

#include "../utils/InputOutput.hpp"

#include <boost/test/included/unit_test.hpp>

using cxint = std::complex<int>;
using veccxint = std::vector<cxint>;

BOOST_AUTO_TEST_CASE(output_vec) {
	std::stringstream s;
	veccxint a = { cxint(1, 2), cxint(2, -4) };
	s << a;
	auto r = s.str();

	BOOST_TEST(r == "(1,2)  (2,-4)  ");
}

BOOST_AUTO_TEST_CASE(output_mat) {
	std::stringstream s;
	std::vector<veccxint> a = { { cxint(1, 2), cxint(2, -4) }, { cxint(2, 3), cxint(3, -5) } };
	s << a;
	auto r = s.str(); 

	BOOST_TEST(r == "(1,2)  (2,-4)  \n(2,3)  (3,-5)  \n");
}

BOOST_AUTO_TEST_CASE(parse_input_line) {
	auto stmp1 = std::istringstream("hello world");
	auto is1 = std::istream(stmp1.rdbuf());
	auto r1 = tech::parse_input_line<std::string>(is1);

	BOOST_TEST(r1[0] == "hello");
	BOOST_TEST(r1[1] == "world");

	auto stmp2 = std::istringstream("hello world");
	auto is2 = std::istream(stmp2.rdbuf());
	auto r2 = tech::parse_input_line<char>(is2);

	BOOST_TEST(r2[0] == 'h');
	BOOST_TEST(r2[5] == ' ');
}

BOOST_AUTO_TEST_CASE(split) {
	std::string s = "1 2 3 4";

	auto r = tech::split(s, ' ');

	BOOST_TEST(r.size() == 4);
	BOOST_TEST(r[0] == "1");
	BOOST_TEST(r[1] == "2");
}

BOOST_AUTO_TEST_CASE(replace) {
	std::string s = "aaa";
	tech::replace(s, "a", "b");
	BOOST_TEST(s == "bbb");
}

BOOST_AUTO_TEST_CASE(is_properly_nested) {

	std::string s1 = "({[a]})", s2 = "(([{aa})]";

	BOOST_TEST(tech::is_properly_nested(s1) == true);
	BOOST_TEST(tech::is_properly_nested(s2) == false);
}

