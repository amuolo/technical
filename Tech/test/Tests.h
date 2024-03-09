#pragma once

#ifndef INCLUDE_TEST_TESTS_H_
#define INCLUDE_TEST_TESTS_H_

#include "IntervalMapTest.h"

void run_all_tests() {
	std::cout << std::endl << "----- RUN ALL TESTS ----- \n" << std::endl;

	IntervalMapTest::eval();
};


#endif

