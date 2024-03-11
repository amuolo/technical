#pragma once

#ifndef INCLUDE_UTILS_INPUTOUTPUT_H_
#define INCLUDE_UTILS_INPUTOUTPUT_H_

#include "StandardLibs.h"

template<class T>
std::vector<T> parse_input_line()
{
    std::string s;
    T item;
    getline(std::cin, s);
    std::stringstream stream(s);
    std::vector<T> input;

    while (stream >> item)
        input.push_back(item);

    return input;
}

inline void print(std::vector<int> arr)
{
    for (auto a : arr)
        std::cout << a << ' ';
};

inline void print(std::vector<std::string> arr)
{
    for (auto a : arr)
        std::cout << a << std::endl;
};

inline std::string print_caller_name(const std::source_location& location = std::source_location::current())
{
    return location.function_name();
}

#endif
