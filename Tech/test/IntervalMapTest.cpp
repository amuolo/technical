#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE IntervalMap

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/IntervalMap.h"

#include <boost/test/included/unit_test.hpp>

static class IntervalMapTest {
public:
	template<class K, class V>
	static void print(const interval_map<K, V>& mymap) {

		std::cout << std::endl;
		for (int i = -3; i < 30; i++) {
			std::cout << std::setw(5) << "  " + std::to_string(i) << "  "
				<< (mymap[i] == 'A' ? "" : (mymap[i] == 'Z' ? "  " : " "))
				<< mymap[i] << std::endl;
		}

		std::cout << std::endl;
		for (auto it : mymap.m_map) {
			std::cout << " " << it.first << " " << it.second << "  ";
		}
	}

	static std::string getMap(const interval_map<int, char>& mymap) {
		std::string r = "";
		for (auto it : mymap.m_map) {
			r += " " + std::to_string(it.first) + " " + it.second + " ";
		}
		return r;
	}
};

BOOST_AUTO_TEST_CASE(insert_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(3, 5, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 Z  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(insert_bis_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(3, 9, 'B');

	mapping.assign(5, 7, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 3 B  5 Z  7 B  9 A "));
}

BOOST_AUTO_TEST_CASE(insert_before_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(1, 3, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 Z  3 A  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(insert_before_bis_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(-5, -2, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" -5 Z  -2 A  1 B  3 A  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(insert_before_tricky_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 5, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(-1, 3, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" -1 Z  3 B  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(insert_complex_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(3, 6, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 Z  6 C  7 A "));
}

BOOST_AUTO_TEST_CASE(canonicity_simple_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');

	mapping.assign(5, 20, 'A');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 A "));
};

BOOST_AUTO_TEST_CASE(canonicity_inject_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 7, 'B');

	mapping.assign(5, 10, 'A');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  5 A "));
};

BOOST_AUTO_TEST_CASE(canonicity_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(15, 23, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 E  15 Z  23 A "));
};

BOOST_AUTO_TEST_CASE(short_intrusion_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(13, 14, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 Z  14 E  17 A  19 F  21 A "));
};

BOOST_AUTO_TEST_CASE(long_intrusion_test) {
	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(6, 20, 'Z');

	BOOST_TEST(IntervalMapTest::getMap(mapping) == std::string(" 1 B  3 A  5 C  6 Z  20 F  21 A "));
};




class myKey {
public:
	int myInt;
	myKey(int i) : myInt(i) {};

	friend bool operator< (const myKey a, const myKey b) { return a.myInt < b.myInt; };
};

class myValue {
public:
	int myInt;
	myValue(int i) : myInt(i) {};

	bool operator== (const myValue item) { return item.myInt == this->myInt; }
};

BOOST_AUTO_TEST_CASE(type_requirements_test) {
	auto valBegin = myValue(0);

	auto mapping = interval_map<myKey, myValue>(valBegin);

	mapping.assign(myKey(1), myKey(3), myValue(2));
	mapping.assign(myKey(5), myKey(7), myValue(4));
}




