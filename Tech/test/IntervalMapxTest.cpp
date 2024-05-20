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

BOOST_AUTO_TEST_CASE(maximum_insert_from_right_4) {
	auto intervals = tech::interval_mapx<myKey, myValue>(tech::insert_mode::maximum, myValue(0));
	intervals.insert({ myKey(0), myKey(5) }, { myKey(4), myKey(8) }, { myValue(1), myValue(2) });    // 0 4  5 8
	intervals.insert(myKey(2), myKey(5), myValue(3));                  // 2 5  5 8

	BOOST_TEST(intervals.count() == 2);
	BOOST_TEST(intervals.contains(myKey(0)) == false);
	BOOST_TEST(intervals.contains(myKey(1)) == false);
	BOOST_TEST(intervals.contains(myKey(2)) == true);
	BOOST_TEST(intervals.contains(myKey(3)) == true);
	BOOST_TEST(intervals.contains(myKey(4)) == true);
	BOOST_TEST(intervals.contains(myKey(5)) == true);
	BOOST_TEST(intervals.contains(myKey(6)) == true);
}