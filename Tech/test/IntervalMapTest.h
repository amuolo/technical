#pragma once

#include "../utils/IntervalMap.h"

static class IntervalMapTest {
public:
	template<class K, class V>
	static void print(const interval_map<K, V>& mymap);

	static std::string test1();
};