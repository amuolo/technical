#pragma once

#ifndef INCLUDE_TEST_TESTS_H_
#define INCLUDE_TEST_TESTS_H_

#include "../utils/InputOutput.h"
#include "IntervalMapTest.h"

std::tuple<int, int> run_all_tests() {
	std::cout << std::endl << "----- RUN ALL TESTS ----- \n" << std::endl;

	auto results = IntervalMapTest::eval();

	std::cout << std::endl << " Summary (succeeded total): ";
	print(results);

	std::cout << std::endl << std::endl;

	return results;
};


#endif

