#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Caching

#include "../utils/Caching.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(caching_lru_1) {
	auto c = tech::cache_lru<int, int>(1);
	c.set(1, 2);
	BOOST_TEST(c.get(1).first == true);   BOOST_TEST(*(c.get(1).second) == 2);
	BOOST_TEST(c.get(2).first == false);
}

BOOST_AUTO_TEST_CASE(caching_lru_2) {
	auto c = tech::cache_lru<int, int>(4);
	c.set(4, 2);
	c.set(2, 7);
	BOOST_TEST(c.get(2).first == true);   BOOST_TEST(*(c.get(2).second) == 7);
	c.set(1, 8);
	c.set(5, 9);
	c.set(6, 15);
	BOOST_TEST(c.get(4).first == false);  
	BOOST_TEST(c.get(5).first == true);   BOOST_TEST(*(c.get(5).second) == 9);
}