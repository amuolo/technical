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

BOOST_AUTO_TEST_CASE(is_properly_nested) {

	std::string s1 = "({[a]})", s2 = "(([{aa})]";

	BOOST_TEST(tech::is_properly_nested(s1) == true);
	BOOST_TEST(tech::is_properly_nested(s2) == false);
}