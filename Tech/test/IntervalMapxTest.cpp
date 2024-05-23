#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalMapx

#include "../utils/IntervalMapx.hpp"

#include <boost/test/included/unit_test.hpp>

class myKey {
public:
	int myInt;
	myKey(int i) : myInt(i) {};

	bool operator < (const myKey& a) const noexcept { return myInt < a.myInt; };

	bool operator <= (const myKey& a) const noexcept { return myInt <= a.myInt; };

	friend myKey operator - (const myKey& a, const myKey& b) { return myKey(a.myInt - b.myInt); }

	friend std::ostream& operator << (std::ostream& o, const myKey& x) {
		o << "myValue: " << std::to_string(x.myInt);
		return o;
	};
};

class myValue {
public:
	int myInt;
	myValue(int i) : myInt(i) {};

	friend bool operator == (const myValue& a, const myValue& b) { return a.myInt == b.myInt; }

	friend std::ostream& operator << (std::ostream& o, const myValue& x) {
		o << "myValue: " << std::to_string(x.myInt);
		return o;
	};
};

/***************/
/*   MAXIMUM   */
/***************/

BOOST_AUTO_TEST_CASE(maximum_insert_d) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(-1), myKey(5), myKey(2), myKey(0), myKey(-4), myKey(-3) }, 
					 { myKey(1), myKey(4), myKey(4), myKey(6), myKey(5), myKey(20), myKey(-2), myKey(-3) },
					 { myValue(1), myValue(2), myValue(3), myValue(4), myValue(5), myValue(6), myValue(7), myValue(8) });
	// -4 -2  3 4  5 6       

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(-4)) == true);   BOOST_TEST(intervals[myKey(-4)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-3)) == true);   BOOST_TEST(intervals[myKey(-3)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-2)) == false);  BOOST_TEST(intervals[myKey(-2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(-1)) == false);  BOOST_TEST(intervals[myKey(-1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(20)) == false);  BOOST_TEST(intervals[myKey(20)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_twice) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(5) }, { myKey(2), myKey(3), myKey(6) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(4), myKey(8), myKey(-1) }, { myKey(4), myKey(12), myKey(1) }, { myValue(4), myValue(5), myValue(6) });
	// 1 2  5 6
	// -1 1  1 2  5 6  8 12

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(-1)) == true);   BOOST_TEST(intervals[myKey(-1)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_tricky_repetitive_inserts) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	// 3 6  8 15
	intervals.insert({ myKey(1), myKey(3), myKey(8) }, { myKey(10), myKey(6), myKey(15) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(0), myKey(2) }, { myKey(1), myKey(3) }, { myValue(4), myValue(5) });  // 0 1  2 3  3 6  8 15
	intervals.insert(myKey(5), myKey(8), myValue(6));
	intervals.insert(myKey(5), myKey(6), myValue(7));                           // 0 1  2 3  5 6  8 15
	intervals.insert(myKey(7), myKey(8), myValue(8));                           // 0 1  2 3  5 6  7 8 15

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(4)); 
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(8));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(12)) == true);   BOOST_TEST(intervals[myKey(12)] == myValue(3));
}

BOOST_AUTO_TEST_CASE(maximum_reaching_end_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(3), myKey(9) }, { myValue(1), myValue(2) });   // 1 3  5 9
	intervals.insert(myKey(8), myKey(10), myValue(3));                // 1 3  8 10

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);   BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(10)) == false);  BOOST_TEST(intervals[myKey(10)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_reaching_begin_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(4), myKey(9) }, { myValue(1), myValue(2) });   // 1 4  5 9
	intervals.insert(myKey(0), myKey(2), myValue(3));                 // 0 2  5 9
	intervals.insert(myKey(-1), myKey(1), myValue(4));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(-1)) == false);  BOOST_TEST(intervals[myKey(-1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_in_consecutive_intervals) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(5), myKey(9) }, { myValue(1), myValue(2) });   // 0 5  5 9
	intervals.insert(myKey(2), myKey(3), myValue(3));   // 2 3  5 9

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_between) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_between_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_inside) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	// 0 2  2 5  6 8
	intervals.insert({ myKey(0), myKey(2), myKey(6) }, { myKey(2), myKey(5), myKey(8) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert(myKey(0), myKey(1), myValue(4));    // 0 1  2 5  6 8
	intervals.insert(myKey(2), myKey(3), myValue(5));    // 0 1  2 3  6 8

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(3));
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(3), myKey(4), myValue(3));                  // 0 3  3 4  5 8
	intervals.insert(myKey(4), myKey(6), myValue(4));                  // 0 3  3 4  4 6
	intervals.insert(myKey(3), myKey(5), myValue(5));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);  BOOST_TEST(intervals[myKey(7)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0) }, { myKey(5) }, { myValue(1) });
	intervals.insert(myKey(3), myKey(5), myValue(2));

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(6), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(4), myKey(8) }, { myValue(1), myValue(2) });    // 0 4  5 8
	intervals.insert(myKey(2), myKey(5), myValue(3));                  // 2 5  5 8

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(myKey(0))) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(myKey(1))) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(myKey(2))) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(myKey(3))) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(myKey(4))) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(myKey(5))) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(myKey(6))) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(maximum_insert_consecutive) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(2) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 0 2  2 5

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_consecutive_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(1) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 0 2

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(3), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_5) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_6) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_7) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  5 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_8) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(9), myValue(5));

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(7)) == true);   BOOST_TEST(intervals[myKey(7)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(8)) == true);   BOOST_TEST(intervals[myKey(8)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(9)) == true);   BOOST_TEST(intervals[myKey(9)] == myValue(4));
}

BOOST_AUTO_TEST_CASE(maximum_insert_neighbours_9) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(12), myValue(5));

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);    BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(10)) == false);  BOOST_TEST(intervals[myKey(10)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(11)) == false);  BOOST_TEST(intervals[myKey(11)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}



/********************/
/*   CONSERVATIVE   */
/********************/

BOOST_AUTO_TEST_CASE(conservative_insert_d) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(-1), myKey(5), myKey(2), myKey(0), myKey(-4), myKey(-3) },
					 { myKey(1), myKey(4), myKey(4), myKey(6), myKey(5), myKey(20), myKey(-2), myKey(-3) },
					 { myValue(1), myValue(2), myValue(3), myValue(4), myValue(5), myValue(6), myValue(7), myValue(8) });
	// -4 -2  3 4  5 6       

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(-4)) == true);   BOOST_TEST(intervals[myKey(-4)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-3)) == true);   BOOST_TEST(intervals[myKey(-3)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-2)) == false);  BOOST_TEST(intervals[myKey(-2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(-1)) == false);  BOOST_TEST(intervals[myKey(-1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(20)) == false);  BOOST_TEST(intervals[myKey(20)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_twice) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(5) }, { myKey(2), myKey(3), myKey(6) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(4), myKey(8), myKey(-1) }, { myKey(4), myKey(12), myKey(1) }, { myValue(4), myValue(5), myValue(6) });
	// 1 2  5 6
	// -1 1  1 2  5 6  8 12

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(-1)) == true);   BOOST_TEST(intervals[myKey(-1)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_tricky_repetitive_inserts) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	// 1 10
	intervals.insert({ myKey(1), myKey(3), myKey(8) }, { myKey(10), myKey(6), myKey(15) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(0), myKey(2) }, { myKey(1), myKey(3) }, { myValue(4), myValue(5) });  // 0 1  1 10
	intervals.insert(myKey(5), myKey(8), myValue(6));
	intervals.insert(myKey(5), myKey(6), myValue(7));                           
	intervals.insert(myKey(7), myKey(8), myValue(8));                           

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(4)) == true);    BOOST_TEST(intervals[myKey(4)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_reaching_end_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(3), myKey(9) }, { myValue(1), myValue(2) });   // 1 3  5 9
	intervals.insert(myKey(8), myKey(10), myValue(3));                // 1 3  5 9

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(9)) == false);   BOOST_TEST(intervals[myKey(9)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(10)) == false);  BOOST_TEST(intervals[myKey(10)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_reaching_begin_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(4), myKey(9) }, { myValue(1), myValue(2) });   // 1 4  5 9
	intervals.insert(myKey(0), myKey(2), myValue(3));                 
	intervals.insert(myKey(-1), myKey(1), myValue(4));   // -1 1  1 4  5 9

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(-1)) == true);   BOOST_TEST(intervals[myKey(-1)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_in_consecutive_intervals) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(5), myKey(9) }, { myValue(1), myValue(2) });   // 0 5  5 9
	intervals.insert(myKey(2), myKey(3), myValue(3));   

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_between) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(4), myValue(3));   // 0 2  3 4  5 9

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_between_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_inside) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	// 0 2  2 5  6 8
	intervals.insert({ myKey(0), myKey(2), myKey(6) }, { myKey(2), myKey(5), myKey(8) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert(myKey(0), myKey(1), myValue(4));    
	intervals.insert(myKey(2), myKey(3), myValue(5));    

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(3));
}

BOOST_AUTO_TEST_CASE(conservative_insert_from_right) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(3), myKey(4), myValue(3));                  // 0 3  3 4  5 8
	intervals.insert(myKey(4), myKey(6), myValue(4));                  
	intervals.insert(myKey(3), myKey(5), myValue(5));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(7)) == true);   BOOST_TEST(intervals[myKey(7)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_from_right_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0) }, { myKey(5) }, { myValue(1) });
	intervals.insert(myKey(3), myKey(5), myValue(2));

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_from_right_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_from_right_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(6), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_from_right_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(4), myKey(8) }, { myValue(1), myValue(2) });   // 0 4  5 8
	intervals.insert(myKey(2), myKey(5), myValue(3));   

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(myKey(0))) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(myKey(1))) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(myKey(2))) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(myKey(3))) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(myKey(4))) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(myKey(5))) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(myKey(6))) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(conservative_insert_consecutive) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(2) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 0 2  2 5

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_consecutive_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(1) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 0 2

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(3), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_5) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_6) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_7) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  5 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_8) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(9), myValue(5));

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(7)) == true);   BOOST_TEST(intervals[myKey(7)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(8)) == true);   BOOST_TEST(intervals[myKey(8)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(9)) == true);   BOOST_TEST(intervals[myKey(9)] == myValue(4));
}

BOOST_AUTO_TEST_CASE(conservative_insert_neighbours_9) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::conservative, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(12), myValue(5));

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);    BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(10)) == false);  BOOST_TEST(intervals[myKey(10)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(11)) == false);  BOOST_TEST(intervals[myKey(11)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}



/*******************/
/*   PROGRESSIVE   */
/*******************/

BOOST_AUTO_TEST_CASE(progressive_insert_d) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(-1), myKey(5), myKey(2), myKey(0), myKey(-4), myKey(-3) },
					 { myKey(1), myKey(4), myKey(4), myKey(6), myKey(5), myKey(7), myKey(-2), myKey(-3) },
					 { myValue(1), myValue(2), myValue(3), myValue(4), myValue(5), myValue(6), myValue(7), myValue(8) });
	// -4 -2  0 7       

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(-4)) == true);   BOOST_TEST(intervals[myKey(-4)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-3)) == true);   BOOST_TEST(intervals[myKey(-3)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(-2)) == false);  BOOST_TEST(intervals[myKey(-2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(-1)) == false);  BOOST_TEST(intervals[myKey(-1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(4)) == true);    BOOST_TEST(intervals[myKey(4)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(20)) == false);  BOOST_TEST(intervals[myKey(20)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_twice) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(1), myKey(3), myKey(5) }, { myKey(2), myKey(3), myKey(6) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(4), myKey(8), myKey(-1) }, { myKey(4), myKey(12), myKey(1) }, { myValue(4), myValue(5), myValue(6) });
	// 1 2  5 6
	// -1 1  1 2  5 6  8 12

	BOOST_TEST(intervals.count() == 4);
	BOOST_TEST(intervals.contains(myKey(-1)) == true);   BOOST_TEST(intervals[myKey(-1)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(6));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);   BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_tricky_repetitive_inserts) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	// 3 6  8 15
	intervals.insert({ myKey(1), myKey(3), myKey(8) }, { myKey(10), myKey(6), myKey(15) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert({ myKey(0), myKey(2) }, { myKey(1), myKey(3) }, { myValue(4), myValue(5) });  // 0 1  2 3  3 6  8 15
	intervals.insert(myKey(5), myKey(8), myValue(6));   // 0 1  2 3  5 8  8 15
	intervals.insert(myKey(5), myKey(6), myValue(7));   // 0 1  2 3  5 6  8 15
	intervals.insert(myKey(7), myKey(8), myValue(8));   // 0 1  2 3  5 6  7 8  8 15

	BOOST_TEST(intervals.count() == 5);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(7));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(8));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(12)) == true);   BOOST_TEST(intervals[myKey(12)] == myValue(3));
}

BOOST_AUTO_TEST_CASE(progressive_reaching_end_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(3), myKey(9) }, { myValue(1), myValue(2) });   // 1 3  5 9
	intervals.insert(myKey(8), myKey(10), myValue(3));                // 1 3  8 10

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);   BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);   BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);   BOOST_TEST(intervals[myKey(7)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(10)) == false);  BOOST_TEST(intervals[myKey(10)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_reaching_begin_iterator) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(1), myKey(5) }, { myKey(4), myKey(9) }, { myValue(1), myValue(2) });   // 1 4  5 9
	intervals.insert(myKey(0), myKey(2), myValue(3));    // 0 2  5 9
	intervals.insert(myKey(-1), myKey(1), myValue(4));   // -1 1  5 9

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(-1)) == true);   BOOST_TEST(intervals[myKey(-1)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == false);    BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == false);    BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == false);    BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_in_consecutive_intervals) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(5), myKey(9) }, { myValue(1), myValue(2) });   // 0 5  5 9
	intervals.insert(myKey(2), myKey(3), myValue(3));   // 2 3  5 9

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);   BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);   BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == false);   BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);   BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_insert_between) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(4), myValue(3));   // 0 2  3 4  5 9

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_insert_between_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(9) }, { myValue(1), myValue(2) });  // 0 2  5 9
	intervals.insert(myKey(3), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_insert_inside) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	// 0 2  2 5  6 8
	intervals.insert({ myKey(0), myKey(2), myKey(6) }, { myKey(2), myKey(5), myKey(8) }, { myValue(1), myValue(2), myValue(3) });
	intervals.insert(myKey(0), myKey(1), myValue(4));   // 0 1  2 5  6 8
	intervals.insert(myKey(2), myKey(3), myValue(5));   // 0 1  2 3  6 8

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(4));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(3));
}

BOOST_AUTO_TEST_CASE(progressive_insert_from_right) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(3), myKey(4), myValue(3));   // 0 3  3 4  5 8
	intervals.insert(myKey(4), myKey(6), myValue(4));   // 0 3  3 4  4 6
	intervals.insert(myKey(3), myKey(5), myValue(5));   // 0 3  3 5

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(7)) == false);  BOOST_TEST(intervals[myKey(7)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_from_right_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0) }, { myKey(5) }, { myValue(1) });
	intervals.insert(myKey(3), myKey(5), myValue(2));

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_from_right_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(4), myValue(3));   // 2 4  5 8

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_insert_from_right_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(3), myKey(8) }, { myValue(1), myValue(2) });    // 0 3  5 8
	intervals.insert(myKey(2), myKey(6), myValue(3));   // 2 6

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_from_right_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(4), myKey(8) }, { myValue(1), myValue(2) });   // 0 4  5 8
	intervals.insert(myKey(2), myKey(5), myValue(3));   // 2 5  5 8

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == false);  BOOST_TEST(intervals[myKey(1)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(2));
}

BOOST_AUTO_TEST_CASE(progressive_insert_consecutive) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(2) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 0 2  2 5

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_consecutive_bis) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(1) }, { myKey(2), myKey(5) }, { myValue(1), myValue(2) });   // 1 5

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_1) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_2) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(3), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == false);  BOOST_TEST(intervals[myKey(3)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_3) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_5) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(1), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 1);
	BOOST_TEST(intervals.contains(myKey(0)) == false);  BOOST_TEST(intervals[myKey(0)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_6) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert(myKey(2), myKey(5), myValue(3));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(5)) == false);  BOOST_TEST(intervals[myKey(5)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_7) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  5 6
	intervals.insert(myKey(3), myKey(4), myValue(3));

	BOOST_TEST(intervals.count() == 3);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == false);  BOOST_TEST(intervals[myKey(2)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(3));
	BOOST_TEST(intervals.contains(myKey(4)) == false);  BOOST_TEST(intervals[myKey(4)] == myValue(0));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(2));
	BOOST_TEST(intervals.contains(myKey(6)) == false);  BOOST_TEST(intervals[myKey(6)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_8) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(9), myValue(5));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);   BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);   BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);   BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == true);   BOOST_TEST(intervals[myKey(3)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(4)) == true);   BOOST_TEST(intervals[myKey(4)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(5)) == true);   BOOST_TEST(intervals[myKey(5)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(6)) == true);   BOOST_TEST(intervals[myKey(6)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(7)) == true);   BOOST_TEST(intervals[myKey(7)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(8)) == true);   BOOST_TEST(intervals[myKey(8)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(9)) == false);  BOOST_TEST(intervals[myKey(9)] == myValue(0));
}

BOOST_AUTO_TEST_CASE(progressive_insert_neighbours_9) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::progressive, myValue(0));
	intervals.insert({ myKey(0), myKey(4) }, { myKey(2), myKey(6) }, { myValue(1), myValue(2) });   // 0 2  4 6
	intervals.insert({ myKey(6), myKey(8) }, { myKey(8), myKey(10) }, { myValue(3), myValue(4) });  // 0 2  4 6  6 8  8 10
	intervals.insert(myKey(2), myKey(12), myValue(5));

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == true);    BOOST_TEST(intervals[myKey(0)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(1)) == true);    BOOST_TEST(intervals[myKey(1)] == myValue(1));
	BOOST_TEST(intervals.contains(myKey(2)) == true);    BOOST_TEST(intervals[myKey(2)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(3)) == true);    BOOST_TEST(intervals[myKey(3)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(4)) == true);    BOOST_TEST(intervals[myKey(4)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(5)) == true);    BOOST_TEST(intervals[myKey(5)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(6)) == true);    BOOST_TEST(intervals[myKey(6)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(7)) == true);    BOOST_TEST(intervals[myKey(7)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(8)) == true);    BOOST_TEST(intervals[myKey(8)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(9)) == true);    BOOST_TEST(intervals[myKey(9)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(10)) == true);   BOOST_TEST(intervals[myKey(10)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(11)) == true);   BOOST_TEST(intervals[myKey(11)] == myValue(5));
	BOOST_TEST(intervals.contains(myKey(12)) == false);  BOOST_TEST(intervals[myKey(12)] == myValue(0));
}


