#pragma once

#ifndef INCLUDE_UTILS_CONCEPTS_H_
#define INCLUDE_UTILS_CONCEPTS_H_

#include "StandardLibs.hpp"


template <typename Any>
auto operator| (Any&& obj, std::invocable<Any> auto&& func) {
	return func(std::forward<Any>(obj));

    // Piping example 
    //auto y = 5 | [](int x) {return x * 2; }
    //           | [](int x) {return x * 3; };
}

#endif
