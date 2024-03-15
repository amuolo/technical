#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Arithmetics

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/Arithmetics.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(simple_sum) {
	auto res = std::make_pair(2, 2) + std::make_pair(1, 3);

	BOOST_TEST(res.first == 3);

	BOOST_TEST(res.second == 5);
}

