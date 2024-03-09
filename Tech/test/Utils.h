#pragma once

#ifndef INCLUDE_TEST_UTILS_H_
#define INCLUDE_TEST_UTILS_H_

#include "../utils/Utils.h"

void assertion(bool ok, std::string funcName) {
	std::cout << std::setw(60) << funcName;
	if (!ok) { std::cout << "   failed" << std::endl; }
	else { std::cout << "   ok" << std::endl; }
}

#endif

