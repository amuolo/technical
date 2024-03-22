#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Arithmetics

#include "../utils/StandardLibs.hpp"
#include "../utils/InputOutput.hpp"
#include "../utils/Arithmetics.hpp"

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

BOOST_AUTO_TEST_CASE(simple_tuple_sum) {
	auto res = std::tuple(2, 2.3, 'a') + std::tuple(3, 1.1, 'b');

	BOOST_TEST(std::get<0>(res) == 5);

	BOOST_TEST(std::get<1>(res) == 3.4);

	BOOST_TEST(std::get<2>(res) == -61);
}

BOOST_AUTO_TEST_CASE(char_pointer_sum) {
	std::string a = "hello", b = " ", c = "world", d = "!";

	auto res = std::tuple(a.data(), c.data()) + std::tuple(b.data(), d.data());

	auto str = std::string(std::get<0>(res)) + std::string(std::get<1>(res));

	BOOST_TEST(str == "hello world!");
}

