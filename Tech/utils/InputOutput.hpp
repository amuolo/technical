#pragma once

#ifndef INCLUDE_UTILS_INPUTOUTPUT_H_
#define INCLUDE_UTILS_INPUTOUTPUT_H_

#include "StandardLibs.hpp"

#include <boost/algorithm/string.hpp>


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
    std::vector<T> parse_input_line(std::istream& istr = std::cin)
    {
        std::string s;
        T item;
        getline(istr, s);

        if constexpr (std::is_same_v<T, char>) {
            return std::vector<char>(s.begin(), s.end());
        }
        else {
            std::stringstream stream(s);
            std::vector<T> input;

            while (stream >> item)
                input.push_back(item);

            return input;
        }
    }
    
    auto split(const std::string& str, char delimiter) -> std::vector<std::string>
    {
        // Option 1: C++20 ranges
        auto to_string = [](auto&& r) -> std::string {
            const auto data = &*r.begin();
            const auto size = static_cast<std::size_t>(std::ranges::distance(r));
            return std::string{ data, size };
        };

        auto range = str |
                     std::ranges::views::split(delimiter) |
                     std::ranges::views::transform(to_string);

        return { std::ranges::begin(range), std::ranges::end(range) };

        // Option 2: Boost algorithm
        //std::vector<std::string> seglist;
        //boost::split(seglist, str, boost::is_any_of(delimiter));
        //return seglist;

        // Option 3: c++14
        //std::stringstream test;
        //std::string segment;
        //std::vector<std::string> seglist;
        //test << str;
        //while (std::getline(test, segment, delimiter))
        //  seglist.push_back(segment);
        //return seglist;
    }

    void replace(std::string& str, const std::string& from, const std::string& to) {
        // Option 1: C++14
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); 
        }

        // Option 2: Boost algorithm
        //boost::replace_all(str, from, to);
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
