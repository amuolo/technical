#pragma once

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include "../Technical.h"

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

#endif
