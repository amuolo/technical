#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE InputOutput

#include "../utils/InputOutput.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(is_properly_nested) {

	std::string s1 = "({[a]})", s2 = "(([{aa})]";

	BOOST_TEST(tech::is_properly_nested(s1) == true);
	BOOST_TEST(tech::is_properly_nested(s2) == false);
}