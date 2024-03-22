#pragma once

#ifndef INCLUDE_UTILS_CALCULATOR_H_
#define INCLUDE_UTILS_CALCULATOR_H_

#include "StandardLibs.hpp"

namespace tech 
{
    int get_greater_common_divisor(int aIn, int bIn) {
        int a = aIn > bIn ? aIn : bIn;
        int b = aIn > bIn ? bIn : aIn;
        while (b != 0) {
            auto residual = a % b;
            a = b;
            b = residual;
        }
        return a;
    }
}

#endif
