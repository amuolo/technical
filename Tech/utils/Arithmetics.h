#pragma once

#ifndef INCLUDE_UTILS_ARITHMETICS_H_
#define INCLUDE_UTILS_ARITHMETICS_H_

#include "StandardLibs.h"

template <class T1, class T2>
inline std::string tech_sum(const T1& a, const T2& b) requires(std::is_same_v<T1, char*>) {
	auto i = a ? std::string(a) : "";
	auto j = b ? std::string(b) : "";
	auto r = i + j;
	return r;
}

template <class T1, class T2>
inline T1 tech_sum(const T1& a, const T2& b) requires(!std::is_same_v<T1, char*> && !std::is_same_v<T2, char*>) {
	return a + b;
}

template <class T1, class T2, class W1, class W2>
inline auto operator+ (const std::pair<T1, T2>& a, const std::pair<W1, W2>& b) {

	return std::pair(tech_sum(a.first, b.first), 
					 tech_sum(a.second, b.second));
};

template <class T1, class T2, class W1, class W2>
inline auto operator+ (const std::tuple<T1, T2>& a, const std::tuple<W1, W2>& b) {

	return std::tuple(tech_sum(std::get<0>(a), std::get<0>(b)), 
					  tech_sum(std::get<1>(a), std::get<1>(b)));
};

template <class T1, class T2, class T3, class W1, class W2, class W3>
inline auto operator+ (const std::tuple<T1, T2, T3>& a, const std::tuple<W1, W2, W3>& b) {

	return std::tuple(tech_sum(std::get<0>(a), std::get<0>(b)), 
					  tech_sum(std::get<1>(a), std::get<1>(b)), 
					  tech_sum(std::get<2>(a), std::get<2>(b)));
}

#endif

