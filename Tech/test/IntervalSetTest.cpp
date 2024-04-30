#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalSet

#include "../utils/IntervalSet.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(simple_intervals) {

	auto intervals = tech::interval_set<int>();
	intervals.assign({ 1, 3, 5 }, { 1, 3, 5 });

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(2) == false);
}

BOOST_AUTO_TEST_CASE(simple_intervals_b) {

	auto intervals = tech::interval_set<int>();
	intervals.assign({ 1, 3, 5 }, { 2, 4, 6 });

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(2) == true);
}


