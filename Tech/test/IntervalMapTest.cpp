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
	long_intrusion_test();
}

void IntervalMapTest::long_intrusion_test() {

	interval_map<int, char> mymap('A');

	mymap.assign(1, 3, 'B');
	mymap.assign(5, 7, 'C');
	mymap.assign(9, 11, 'D');
	mymap.assign(13, 17, 'E');
	mymap.assign(19, 21, 'F');

	mymap.assign(6, 20, 'Z');

	assertion(getMap(mymap) == " 1 B  3 A  5 C  6 Z  20 F  21 A ", std::source_location::current().function_name());
};
