#pragma once

#ifndef INCLUDE_TEST_UTILS_H_
#define INCLUDE_TEST_UTILS_H_

#include "../utils/StandardLibs.h"
#include "../utils/Arithmetics.h"

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

std::tuple<int, int> eval_tests(std::vector<std::function<void()>> tests) {
	int num_succeeded = 0;
	for (auto& test : tests) {
		try {
			test();
			num_succeeded++;
			std::cout << "   ok" << std::endl;
		}
		catch (std::exception e) {
			std::cout << "   failed \n\n" << e.what() << std::endl;
		}
	}
	return std::make_pair(num_succeeded, tests.size()) + std::make_pair(1, 1);
}

#endif

