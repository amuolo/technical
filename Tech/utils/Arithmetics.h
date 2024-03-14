#pragma once

#ifndef INCLUDE_UTILS_ARITHMETICS_H_
#define INCLUDE_UTILS_ARITHMETICS_H_

#include "StandardLibs.h"

template <class T1, class T2, class W1, class W2>
std::pair<T1, T2> operator+ (std::pair<T1, T2> a, std::pair<W1, W2> b) {
	return std::make_pair(a.first + b.first, a.second + b.second);
};

#endif
