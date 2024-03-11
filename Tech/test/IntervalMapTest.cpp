#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "IntervalMapTest.h"
#include "Utils.h"

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
		insert_test,
		insert_bis_test,
		insert_before_test,
		insert_before_bis_test,
		insert_before_tricky_test,
		insert_complex_test,
		canonicity_simple_test,
		canonicity_inject_test,
		canonicity_test,
		short_intrusion_test,
		long_intrusion_test,
		type_requirements_test
	});
}

void IntervalMapTest::insert_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(3, 5, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 Z  5 C  7 A "));
}

void IntervalMapTest::insert_bis_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(3, 9, 'B');

	mapping.assign(5, 7, 'Z');

	assertion(getMap(mapping), std::string(" 3 B  5 Z  7 B  9 A "));
}

void IntervalMapTest::insert_before_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(1, 3, 'Z');

	assertion(getMap(mapping), std::string(" 1 Z  3 A  5 C  7 A "));
}

void IntervalMapTest::insert_before_bis_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(-5, -2, 'Z');

	assertion(getMap(mapping), std::string(" -5 Z  -2 A  1 B  3 A  5 C  7 A "));
}

void IntervalMapTest::insert_before_tricky_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 5, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(-1, 3, 'Z');

	assertion(getMap(mapping), std::string(" -1 Z  3 B  5 C  7 A "));
}

void IntervalMapTest::insert_complex_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');

	mapping.assign(3, 6, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 Z  6 C  7 A "));
}

void IntervalMapTest::canonicity_simple_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');

	mapping.assign(5, 20, 'A');

	assertion(getMap(mapping), std::string(" 1 B  3 A "));
};

void IntervalMapTest::canonicity_inject_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 7, 'B');

	mapping.assign(5, 10, 'A');

	assertion(getMap(mapping), std::string(" 1 B  5 A "));
};

void IntervalMapTest::canonicity_test() {
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(15, 23, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 A  5 C  7 A  9 D  11 A  13 E  15 Z  23 A "));
};

void IntervalMapTest::short_intrusion_test() {
	assertion(print_caller_name());

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
	assertion(print_caller_name());

	auto mapping = interval_map<int, char>('A');

	mapping.assign(1, 3, 'B');
	mapping.assign(5, 7, 'C');
	mapping.assign(9, 11, 'D');
	mapping.assign(13, 17, 'E');
	mapping.assign(19, 21, 'F');

	mapping.assign(6, 20, 'Z');

	assertion(getMap(mapping), std::string(" 1 B  3 A  5 C  6 Z  20 F  21 A "));
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

void IntervalMapTest::type_requirements_test() {
	assertion(print_caller_name());

	auto valBegin = myValue(0);

	auto mapping = interval_map<myKey, myValue>(valBegin);

	mapping.assign(myKey(1), myKey(3), myValue(2));
	mapping.assign(myKey(5), myKey(7), myValue(4));

	assertion(1, 1);
}

