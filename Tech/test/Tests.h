#pragma once

#ifndef INCLUDE_TEST_TESTS_H_
#define INCLUDE_TEST_TESTS_H_

#include "IntervalMapTest.h"

std::tuple<int, int> run_all_tests() {
	std::cout << std::endl << "----- RUN ALL TESTS ----- \n" << std::endl;

	return IntervalMapTest::eval();
};


#endif

