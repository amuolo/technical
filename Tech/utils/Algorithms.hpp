#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMS_H_
#define INCLUDE_UTILS_ALGORITHMS_H_

#include "StandardLibs.hpp"

#include <boost/type_traits.hpp>

namespace tech::algorithms
{
    template <class T>
    inline std::vector<size_t> sort_index(const std::vector<T>& v)
    {
        // initialize original index locations
        std::vector<size_t> idx(v.size());
        std::iota(idx.begin(), idx.end(), 0);

        // sort indexes based on comparing values in v using std::stable_sort instead of std::sort
        // to avoid unnecessary index re-orderings when v contains elements of equal values
        if constexpr (!boost::is_complex<T>::value)
            std::stable_sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
        else
            std::stable_sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return real(v[i1]) < real(v[i2]); });

        return idx;
    }
}

#endif

