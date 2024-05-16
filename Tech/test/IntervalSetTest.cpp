#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalSet

#include "../utils/IntervalSet.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(invalid_argument) {
	
	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	bool excpt = false;
	try {
		intervals.insert({ 1, 3, 5 }, { 1, 3 });
	}
	catch (std::exception e)
	{
		excpt = true;
	}
	BOOST_TEST(excpt == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_a) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.insert({ 1, 4, 5, -1, 0 }, { 1, 3, 6, -1, 0 });

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(-1) == false);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(5) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_b) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.insert({ 1, 3, 5 }, { 2, 4, 6 });

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_c) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.insert({ 1, 3, 5 }, { 10, 4, 6 });

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_d) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.insert({ 1, 3, -1, 5, 2, 0, -4, -3 }, { 1, 4, 4, 6, 5, 20, -2, -3 });
	//                    a      a		   a             z     z          z      

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(-4) == true);
	BOOST_TEST(intervals.contains(-3) == true);
	BOOST_TEST(intervals.contains(-2) == false);
	BOOST_TEST(intervals.contains(-1) == false);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == false);
	BOOST_TEST(intervals.contains(20) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_twice) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.insert({ 1, 3, 5 }, { 2, 3, 6 });     // 1 2  5 6
	intervals.insert({ 4, 8, -1 }, { 4, 12, 1 });   // -1 1 2  5 6  8 12

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(-1) == true);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == false);
	BOOST_TEST(intervals.contains(8) == true);
	BOOST_TEST(intervals.contains(12) == false);
}


