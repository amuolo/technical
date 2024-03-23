#pragma once

#ifndef INCLUDE_UTILS_CALCULATOR_H_
#define INCLUDE_UTILS_CALCULATOR_H_

#include "StandardLibs.hpp"

namespace tech 
{
    template <class T>
    T get_greater_common_divisor(T aIn, T bIn) requires(std::is_integral_v<T>) {
        auto a = aIn > bIn ? aIn : bIn;
        auto b = aIn > bIn ? bIn : aIn;
        while (b != 0) {
            auto residual = a % b;
            a = b;
            b = residual;
        }
        return a;
    }

    template <class T>
    std::vector<T> get_primes(T max) requires(std::is_integral_v<T>) {
        auto res = std::vector<T>{ 2,3,5,7,11,13,17,19,23,29,31,37 };
        for (T n = 41; n <= max; n++) {
            bool ok = true;
            T sqrtn = std::sqrt(n);
            for (T prime : res) {
                if (n % prime == 0) { ok = false; break; }
                if (prime > sqrtn) break;
            }
            if (ok) res.push_back(n);
        }
        return res;
    }
}

#endif
