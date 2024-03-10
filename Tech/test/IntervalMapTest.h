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

	static void short_intrusion_test();
	static void long_intrusion_test();

	static void non_default_constructible_test();
};


#endif
