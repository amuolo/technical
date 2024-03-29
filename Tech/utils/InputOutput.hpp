#pragma once

#ifndef INCLUDE_UTILS_INPUTOUTPUT_H_
#define INCLUDE_UTILS_INPUTOUTPUT_H_

#include "StandardLibs.hpp"

template <class T>
inline std::ostream& operator << (std::ostream& c, const std::vector<T>& x)
{
    for (size_t i = 0; i < x.size(); i++) {
        c << x[i] << "  ";
    }
    return c;
}

namespace tech
{
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

    bool is_properly_nested(std::string s)
    {
        if (s.size() == 0) return 1;

        char a1 = '(', a2 = '[', a3 = '{', b1 = ')', b2 = ']', b3 = '}';

        std::vector<char> a = { a1, a2, a3 }, b = { b1, b2, b3 }, z = {};

        for(const auto& ch : s) {
            if (std::find(b.begin(), b.end(), ch) == b.end() && std::find(a.begin(), a.end(), ch) == a.end()) 
                continue;
            else if (z.size() == 0 && std::find(b.begin(), b.end(), ch) != b.end()) 
                return 0;
            else if (std::find(a.begin(), a.end(), ch) != a.end()) 
                z.push_back(ch);
            else if (ch == b3 && z.back() == a3) 
                z.pop_back();
            else if (ch == b2 && z.back() == a2) 
                z.pop_back();
            else if (ch == b1 && z.back() == a1) 
                z.pop_back();
            else 
                return 0;
        }

        return z.size() == 0 ? 1 : 0;
    }

    template <class T>
    inline void print(std::vector<T> arr) requires(std::is_arithmetic_v<T>)
    {
        for (const auto& a : arr)
            std::cout << a << ' ';
    };

    inline void print(std::vector<std::string> arr)
    {
        for (const auto& a : arr)
            std::cout << a << std::endl;
    };

    template <class K, class V>
    inline void print(std::map<K, V> map)
    {
        for (const auto& it : map)
            std::cout << " " << it.first << " " << it.second << "  ";
    }

    template <class T1, class T2>
    inline void print(std::tuple<T1, T2> item)
    {
        std::cout << std::get<0>(item) << " " << std::get<1>(item);
    }

    template <class T1, class T2, class T3>
    inline void print(std::tuple<T1, T2, T3> item)
    {
        std::cout << std::get<0>(item) << " " << std::get<1>(item) << " " << std::get<2>(item);
    }

    inline std::string get_caller_name(const std::source_location& location = std::source_location::current())
    {
        return location.function_name();
    }

}

#endif
