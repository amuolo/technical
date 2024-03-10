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
		std::stringstream s;
		
		if constexpr (std::is_same_v<T, std::string> || std::is_integral_v<T>) {
			s << std::setw(15) << "value: " << value << std::endl
			  << std::setw(15) << "benchmark: " << benchmark << std::endl;
		}

		throw std::exception(s.str().c_str());
	}
}

void eval_tests(std::vector<std::function<void()>> tests) {
	for (auto& test : tests) {
		try {
			test();
			std::cout << "   ok" << std::endl;
		}
		catch (std::exception e) {
			std::cout << "   failed \n\n" << e.what() << std::endl;
		}
	}
}

#endif

