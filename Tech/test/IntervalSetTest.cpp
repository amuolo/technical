#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalSet

#include "../utils/IntervalSet.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(invalid_argument) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
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
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
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
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 1, 3, 5 }, { 2, 4, 6 });

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_c) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
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
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
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
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
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

BOOST_AUTO_TEST_CASE(maximum_tricky_repetitive_inserts) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 1, 3, 8 }, { 10, 6, 15 });     // 3 6  8 15
	intervals.insert({ 0, 2 }, { 1, 3 });             // 0 1  2 3  3 6  8 15
	intervals.insert(5, 8);
	intervals.insert(5, 6);                           // 0 1  2 3  5 6  8 15
	intervals.insert(7, 8);                           // 0 1  2 3  5 6  7 8 15

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == true);
	BOOST_TEST(intervals.contains(8) == true);
	BOOST_TEST(intervals.contains(9) == true);
	BOOST_TEST(intervals.contains(12) == true);
}

BOOST_AUTO_TEST_CASE(maximum_reaching_end_iterator) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 1, 5 }, { 3, 9 });   // 1 3  5 9
	intervals.insert(8, 10);                // 1 3  8 10

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == false);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == false);
	BOOST_TEST(intervals.contains(8) == true);
	BOOST_TEST(intervals.contains(9) == true);
	BOOST_TEST(intervals.contains(10) == false);
}

BOOST_AUTO_TEST_CASE(maximum_reaching_begin_iterator) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 1, 5 }, { 4, 9 });   // 1 4  5 9
	intervals.insert(0, 2);                 // 0 2  5 9
	intervals.insert(-1, 1);

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
}

BOOST_AUTO_TEST_CASE(maximum_tricky_insertion_sequence) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ -5, 5 }, { 5, 8 });   // -5 5  5 8
	intervals.insert(0, 5);                  //  0 5  5 8
	intervals.insert(1, 4);                  //  1 4  5 8
	intervals.insert(3, 7);

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
	BOOST_TEST(intervals.contains(8) == false);
}

BOOST_AUTO_TEST_CASE(maximum_chain_of_intervals) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ -6, -5, 2, 5, 8 }, { -5, 2, 5, 8, 10 });   // -6 -5  -5 2  2 5  5 8  8 10
	intervals.insert(9, 10);                                      // -6 -5  -5 2  2 5  5 8  9 10
	intervals.insert(7, 9);                                       // -6 -5  -5 2  2 5  7 9  9 10
	intervals.insert(2, 4);                                       // -6 -5  -5 2  2 4  7 9  9 10
	intervals.insert(-4, 0);                                      // -6 -5  -4 0  2 4  7 9  9 10

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.contains(-6) == true);
	BOOST_TEST(intervals.contains(-5) == false);
	BOOST_TEST(intervals.contains(-4) == true);
	BOOST_TEST(intervals.contains(-3) == true);
	BOOST_TEST(intervals.contains(-2) == true);
	BOOST_TEST(intervals.contains(-1) == true);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == false);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == true);
	BOOST_TEST(intervals.contains(8) == true);
	BOOST_TEST(intervals.contains(9) == true);
	BOOST_TEST(intervals.contains(10) == false);
}

BOOST_AUTO_TEST_CASE(maximum_in_consecutive_intervals) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 5, 9 });
	intervals.insert(2, 3);

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_between) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 2, 9 });
	intervals.insert(3, 4);

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_between_bis) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 2, 9 });
	intervals.insert(3, 5);

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == true);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_inside) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 2, 6 }, { 2, 5, 8 });    // 0 2  2 5  6 8
	intervals.insert(0, 1);
	intervals.insert(2, 3);                        // 0 1  2 3  6 8

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == false);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 3, 8 });    // 0 3  5 8
	intervals.insert(3, 4);                  // 0 3  3 4  5 8
	intervals.insert(4, 6);                  // 0 3  3 4  4 6
	intervals.insert(3, 5);

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == true);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == false);
	BOOST_TEST(intervals.contains(7) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_1) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0 }, { 5 });
	intervals.insert(3, 5);

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == false);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == true);
	BOOST_TEST(intervals.contains(5) == false);
	BOOST_TEST(intervals.contains(6) == false);
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_2) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 3, 8 });    // 0 3  5 8
	intervals.insert(2, 4);

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_3) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 3, 8 });    // 0 3  5 8
	intervals.insert(2, 6);

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == true);
	BOOST_TEST(intervals.contains(1) == true);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == false);
	BOOST_TEST(intervals.contains(4) == false);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_4) {
	auto intervals = tech::interval_set<int>(tech::insert_mode::maximum);
	intervals.insert({ 0, 5 }, { 4, 8 });    // 0 4  5 8
	intervals.insert(2, 5);                  // 2 5  5 8

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(0) == false);
	BOOST_TEST(intervals.contains(1) == false);
	BOOST_TEST(intervals.contains(2) == true);
	BOOST_TEST(intervals.contains(3) == true);
	BOOST_TEST(intervals.contains(4) == true);
	BOOST_TEST(intervals.contains(5) == true);
	BOOST_TEST(intervals.contains(6) == true);
}
 

