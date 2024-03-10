#include "IntervalMapTest.h"
#include "Utils.h"

#include <assert.h>

template<class K, class V>
void IntervalMapTest::print(const interval_map<K, V>& mymap) {
	
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

std::string IntervalMapTest::getMap(const interval_map<int, char>& mymap) {
	std::string r = "";
	for (auto it : mymap.m_map) {
		r += " " + std::to_string(it.first) + " " + it.second + " ";
	}
	return r;
}

void IntervalMapTest::eval() {
	std::cout << "  --- IntervalMap Tests --- \n" << std::endl;

	eval_tests({
		short_intrusion_test,
		long_intrusion_test,
		non_default_constructible_test
		}
	);
}

void IntervalMapTest::short_intrusion_test() {
	assertion(std::source_location::current().function_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(13, 14, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 Z  14 E  17 A  19 F  21 A "));
};

void IntervalMapTest::long_intrusion_test() {
	assertion(std::source_location::current().function_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(6, 20, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 A  5 C  6 Z  20 F  21 A "));
};

void IntervalMapTest::non_default_constructible_test() {
	assertion(std::source_location::current().function_name());

	class myObject {
	public:
		int myInt;
		myObject(int i) { myInt = i; };

		bool operator== (myObject item) { return item.myInt == this->myInt; }
	};

	auto valBegin = myObject(1);

	auto mapping = interval_map<int, myObject>(valBegin);

	mapping.assign(1, 3, myObject(2));
	mapping.assign(5, 7, myObject(4));

	assertion(1, 1);
}

