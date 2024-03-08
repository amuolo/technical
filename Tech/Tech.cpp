// Technical.cpp : Defines the entry point for the application.
//

#include "Tech.h"
#include "utils/IntervalMap.h"
#include "utils/Utils.h"

using namespace std;

int main()
{
	interval_map<int, char> mymap('A');

	mymap.assign(1, 3, 'B');
	mymap.assign(5, 7, 'C');
	mymap.assign(9, 11, 'D');
	mymap.assign(13, 17, 'E');
	mymap.assign(19, 21, 'F');

	mymap.assign(6, 20, 'Z');

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

	std::cout << std::endl << std::endl;
	return 0;
}
