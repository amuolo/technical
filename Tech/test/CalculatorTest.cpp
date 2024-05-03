#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Calculator

#include "../utils/Calculator.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(gcd) {

	BOOST_TEST(tech::calculator::get_greater_common_divisor(3, 9) == 3);

	BOOST_TEST(tech::calculator::get_greater_common_divisor(5, 3) == 1);

	BOOST_TEST(tech::calculator::get_greater_common_divisor(100, 1) == 1);

	BOOST_TEST(tech::calculator::get_greater_common_divisor(15, 30) == 15);

	BOOST_TEST(tech::calculator::get_greater_common_divisor(15, 21) == 3);
}

BOOST_AUTO_TEST_CASE(primes) {

	BOOST_TEST(tech::calculator::get_primes(100).size() == 25);
}

