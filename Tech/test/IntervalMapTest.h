#pragma once

#ifndef INCLUDE_TEST_INTERVALMAPTEST_H_
#define INCLUDE_TEST_INTERVALMAPTEST_H_

#include "../utils/IntervalMap.h"

static class IntervalMapTest {
public:
	template<class K, class V>
	static void print(const interval_map<K, V>& mymap);

	static std::string getMap(const interval_map<int, char>& mymap);

	static void eval();

	static void insert_test();
	static void insert_bis_test();
	static void insert_before_test();
	static void insert_before_bis_test();
	static void insert_before_tricky_test();
	static void insert_complex_test();
	static void canonicity_simple_test();
	static void canonicity_inject_test();
	static void canonicity_test();
	static void short_intrusion_test();
	static void long_intrusion_test();

	static void type_requirements_test();
};


#endif
