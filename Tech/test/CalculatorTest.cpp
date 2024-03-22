#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Calculator

#include "../utils/StandardLibs.hpp"
#include "../utils/Arithmetics.hpp"
#include "../utils/calculator.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(simple_gcd) {

	BOOST_TEST(3 == 3);
}