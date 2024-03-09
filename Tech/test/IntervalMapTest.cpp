#include "IntervalMapTest.h"

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

std::string IntervalMapTest::test1() {

	interval_map<int, char> mymap('A');

	mymap.assign(1, 3, 'B');
	mymap.assign(5, 7, 'C');
	mymap.assign(9, 11, 'D');
	mymap.assign(13, 17, 'E');
	mymap.assign(19, 21, 'F');

	mymap.assign(6, 20, 'Z');

	print(mymap);

	return "ok";
};
