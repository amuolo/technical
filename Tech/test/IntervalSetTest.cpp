#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalSet

#include "../utils/IntervalSet.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(invalid_argument) {
	
	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	bool excpt = false;
	try {
		intervals.assign({ 1, 3, 5 }, { 1, 3 });
	}
	catch (std::exception e)
	{
		excpt = true;
	}
	BOOST_TEST(excpt == true);
}

BOOST_AUTO_TEST_CASE(maximum_assign_a) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.assign({ 1, 3, 5, -1, 0 }, { 1, 3, 5, -1, 0 });

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(2) == false);
	BOOST_TEST(intervals.IsIn(-1) == true);
	BOOST_TEST(intervals.IsIn(0) == true);
}

BOOST_AUTO_TEST_CASE(maximum_assign_b) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.assign({ 1, 3, 5 }, { 2, 4, 6 });

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(2) == true);
	BOOST_TEST(intervals.IsIn(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_assign_c) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.assign({ 1, 3, 5 }, { 10, 4, 6 });

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.IsIn(1) == false);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(5) == true);
	BOOST_TEST(intervals.IsIn(2) == false);
	BOOST_TEST(intervals.IsIn(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_assign_d) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.assign({ 1, 3, -1, 5, 2, 0, -4, -3 }, { 1, 4, 4, 6, 5, 20, -2, -3 });
	//                 a  a      a			   a      z  z     z              z

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.IsIn(-4) == false);
	BOOST_TEST(intervals.IsIn(-3) == true);
	BOOST_TEST(intervals.IsIn(-2) == false);
	BOOST_TEST(intervals.IsIn(-1) == false);
	BOOST_TEST(intervals.IsIn(0) == false);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(2) == false);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(4) == true);
	BOOST_TEST(intervals.IsIn(5) == true);
	BOOST_TEST(intervals.IsIn(6) == true);
	BOOST_TEST(intervals.IsIn(7) == false);
	BOOST_TEST(intervals.IsIn(20) == false);
}

BOOST_AUTO_TEST_CASE(maximum_assign_twice) {

	auto intervals = tech::interval_set<int>(tech::interval_set_mode::maximum);
	intervals.assign({ 1, 3, 5 }, { 2, 3, 6 });
	intervals.assign({ 4, 8, -1 }, { 4, 12, 1 });

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.IsIn(-1) == true);
	BOOST_TEST(intervals.IsIn(0) == true);
	BOOST_TEST(intervals.IsIn(1) == true);
	BOOST_TEST(intervals.IsIn(2) == false);
	BOOST_TEST(intervals.IsIn(3) == true);
	BOOST_TEST(intervals.IsIn(4) == true);
	BOOST_TEST(intervals.IsIn(5) == true);
	BOOST_TEST(intervals.IsIn(6) == true);
	BOOST_TEST(intervals.IsIn(7) == false);
	BOOST_TEST(intervals.IsIn(8) == true);
	BOOST_TEST(intervals.IsIn(12) == true);
}


