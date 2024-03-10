#pragma once

#ifndef INCLUDE_TEST_UTILS_H_
#define INCLUDE_TEST_UTILS_H_

#include "../utils/Utils.h"

void assertion(std::string funcName) {
	std::cout << std::setw(80) << funcName;
}

template <class T>
void assertion(T value, T benchmark) {
	if (value != benchmark) { 
		std::cout << "   failed" << std::endl; 
		return;
	}
	
	std::cout << "   ok" << std::endl;
}

void eval_tests(std::vector<std::function<void()>> tests) {
	for (auto& test : tests) {
		try {
			test();
		}
		catch (std::exception e) {
			std::cout << "   failed \n\n" << e.what() << std::endl << std::endl;
		}
	}
}

#endif

