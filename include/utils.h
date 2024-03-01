/*
 * utils.h
 *
 *  Created on: 1 Mar 2024
 *      Author: amuolo
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

template<class T>
std::vector<T> parse_input_line()
{
    string s;
    T item;
    getline( cin, s );
    stringstream stream(s);
    std::vector<T> input;
    
    while(stream >> item)        
        input.push_back(item);
   
    return input;
}

auto print = [](std::vector<int> arr)
{
    for (auto a : arr)
        std::cout << a << ' ';
};

auto print = [](std::vector<string> arr)
{
    for (auto a : arr)
	std::cout << a << endl;
};

#endif /* INCLUDE_UTILS_H_ */
