#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE IntervalMap

#include "../utils/StandardLibs.hpp"
#include "../utils/InputOutput.hpp"
#include "../utils/IntervalMap.hpp"

#include <boost/test/included/unit_test.hpp>

namespace tech
{
	static class IntervalMapTest {
	public:
		template<class K, class V>
		static void print(const tech::interval_map<K, V>& mymap) {

			std::cout << std::endl;
			for (int i = -3; i < 30; i++) {
				std::cout << std::setw(5) << "  " + std::to_string(i) << "  "
					<< (mymap[i] == 'A' ? "" : (mymap[i] == 'Z' ? "  " : " "))
					<< mymap[i] << std::endl;
			}

			std::cout << std::endl;
			for (const auto& it : mymap.m_map) {
				std::cout << " " << it.first << " " << it.second << "  ";
			}
		}

		static std::string getMap(const tech::interval_map<int, char>& mymap) {
			std::string r = "";
			for (const auto& it : mymap.m_map) {
				r += " " + std::to_string(it.first) + " " + it.second + " ";
			}
			return r;
		}
	};
}

BOOST_AUTO_TEST_CASE(assign_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');

	intmap.assign(3, 5, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 Z  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(assign_bis_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(3, 9, 'B');

	intmap.assign(5, 7, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 3 B  5 Z  7 B  9 A "));
}

BOOST_AUTO_TEST_CASE(assign_before_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');

	intmap.assign(1, 3, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 Z  3 A  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(assign_before_bis_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');

	intmap.assign(-5, -2, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" -5 Z  -2 A  1 B  3 A  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(assign_before_tricky_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 5, 'B');
	intmap.assign(5, 7, 'C');

	intmap.assign(-1, 3, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" -1 Z  3 B  5 C  7 A "));
}

BOOST_AUTO_TEST_CASE(assign_complex_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');

	intmap.assign(3, 6, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 Z  6 C  7 A "));
}

BOOST_AUTO_TEST_CASE(canonicity_simple_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');

	intmap.assign(5, 20, 'A');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 A "));
};

BOOST_AUTO_TEST_CASE(canonicity_inject_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 7, 'B');

	intmap.assign(5, 10, 'A');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  5 A "));
};

BOOST_AUTO_TEST_CASE(canonicity_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');
	intmap.assign(9, 11, 'D');
	intmap.assign(13, 17, 'E');
	intmap.assign(19, 21, 'F');

	intmap.assign(15, 23, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 E  15 Z  23 A "));
};

BOOST_AUTO_TEST_CASE(short_intrusion_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');
	intmap.assign(9, 11, 'D');
	intmap.assign(13, 17, 'E');
	intmap.assign(19, 21, 'F');

	intmap.assign(13, 14, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 Z  14 E  17 A  19 F  21 A "));
};

BOOST_AUTO_TEST_CASE(long_intrusion_test) {
	auto intmap = tech::interval_map<int, char>('A');

	intmap.assign(1, 3, 'B');
	intmap.assign(5, 7, 'C');
	intmap.assign(9, 11, 'D');
	intmap.assign(13, 17, 'E');
	intmap.assign(19, 21, 'F');

	intmap.assign(6, 20, 'Z');

	BOOST_TEST(tech::IntervalMapTest::getMap(intmap) == std::string(" 1 B  3 A  5 C  6 Z  20 F  21 A "));
};




class myKey {
public:
	int myInt;
	myKey(int i) : myInt(i) {};

	friend bool operator < (const myKey& a, const myKey& b) { return a.myInt < b.myInt; };

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

BOOST_AUTO_TEST_CASE(type_requirements_test) {
	auto valBegin = myValue(0);

	auto intmap = tech::interval_map<myKey, myValue>(valBegin);

	intmap.assign(myKey(1), myKey(3), myValue(2));
	intmap.assign(myKey(5), myKey(7), myValue(4));
	intmap.assign(myKey(2), myKey(6), myValue(6));
	
	BOOST_TEST(intmap[myKey(0)] == myValue(0));
	BOOST_TEST(intmap[myKey(1)] == myValue(2));
	BOOST_TEST(intmap[myKey(2)] == myValue(6));
	BOOST_TEST(intmap[myKey(3)] == myValue(6));
	BOOST_TEST(intmap[myKey(4)] == myValue(6));
	BOOST_TEST(intmap[myKey(5)] == myValue(6));
	BOOST_TEST(intmap[myKey(6)] == myValue(4));
	BOOST_TEST(intmap[myKey(7)] == myValue(0));
	BOOST_TEST(intmap[myKey(8)] == myValue(0));
}




