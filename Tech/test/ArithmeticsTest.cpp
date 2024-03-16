#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Arithmetics

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/Arithmetics.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(simple_pair_sum) {
	auto res = std::make_pair(2, 2) + std::make_pair(1, 3);

	BOOST_TEST(res.first == 3);

	BOOST_TEST(res.second == 5);
}

BOOST_AUTO_TEST_CASE(complex_pair_sum) {
	auto res = std::make_pair(2.3, 'x') + std::make_pair(4.5, 'y');

	BOOST_TEST(res.first == 6.8);

	BOOST_TEST(res.second == -15);
}

