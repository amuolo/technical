/*
 * utils.h
 *
 *  Created on: 1 Mar 2024
 *      Author: amuolo
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

template<class T>
std::vector<T> parse_input_line()
{
    std::string s;
    T item;
    getline( std::cin, s );
    std::stringstream stream(s);
    std::vector<T> input;
    
    while(stream >> item)        
        input.push_back(item);
   
    return input;
}

void print(std::vector<int> arr)
{
    for (auto a : arr)
        std::cout << a << ' ';
};

void print(std::vector<std::string> arr)
{
    for (auto a : arr)
	std::cout << a << std::endl;
};

#endif /* INCLUDE_UTILS_H_ */



