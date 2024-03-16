#pragma once

#ifndef INCLUDE_UTILS_ARITHMETICS_H_
#define INCLUDE_UTILS_ARITHMETICS_H_

#include "StandardLibs.h"

template <class T1, class T2, class W1, class W2>
std::pair<T1, T2> operator+ (const std::pair<T1, T2>& a, const std::pair<W1, W2>& b) {
	return std::pair(a.first + b.first, a.second + b.second);
};

#endif
