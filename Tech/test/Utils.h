#pragma once

#ifndef INCLUDE_TEST_UTILS_H_
#define INCLUDE_TEST_UTILS_H_

#include "../utils/StandardLibs.h"

void assertion(std::string funcName) {
	std::regex re("void|__cdecl");
	auto name = std::regex_replace(funcName, re, "");
	std::cout << std::left << std::setw(70) << name;
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
			//assertion(test.target_type().name());
			test();
			std::cout << "   ok" << std::endl;
		}
		catch (std::exception e) {
			std::cout << "   failed \n\n" << e.what() << std::endl;
		}
	}
}

#endif

