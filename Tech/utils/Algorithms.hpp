#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMS_H_
#define INCLUDE_UTILS_ALGORITHMS_H_

#include "StandardLibs.hpp"

#include <boost/type_traits.hpp>

namespace tech::algorithms
{
    /***************/
    /*   SORTING   */
    /***************/

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

    template <typename T>
    inline std::vector<T> sort_nlogn_counting(const std::vector<T>& v) requires(std::is_integral_v<T>) {
        std::map<T, size_t> counter;
        for (const auto& x : v) {               // O(n)
            const auto& it = counter.find(x);   // O(log(n))
            if (it == counter.end())
                counter[x] = 1;                 // O(log(n))
            else
                counter[x]++;                   // O(log(n))
        }

        std::vector<T> res;
        res.reserve(v.size());
        for (auto it = counter.begin(); it != counter.end(); it++)    // O(log(n))
            for (size_t i = 0; i < it->second; i++)
                res.push_back(it->first);

        return res;
    }

    template <typename T>
    inline std::vector<T> sort_linear_counting_highmem(const std::vector<T>& v) requires(std::is_integral_v<T>) {
        if (v.size() == 0)
            return v;
        size_t max = 0, min = 0;
        std::for_each(v.begin(), v.end(), [&](const auto& x) {   // O(n)
            if (x < 0 && abs(x) > min)
                min = abs(x); 
            else if (x > 0 && x > max)
                max = x; 
        });
        size_t k = 1 + max + min;
        std::vector<size_t> counter(k, 0);
        std::for_each(v.begin(), v.end(), [&](const auto& x) { counter[x + min]++; });   // O(n)
        std::vector<T> res;
        res.reserve(v.size());
        T x = -T(min);
        std::for_each(counter.begin(), counter.end(), [&](const auto& count) {   // O(n)
            for (size_t i = 0; i < count; i++)
                res.push_back(x);
            x++;
        });
        return res;
    }

    /****************/
    /*   SEQUENCE   */
    /****************/

    template <typename T>
    inline std::vector<T> longest_common_subsequence(const std::vector<T>& a, const std::vector<T>& b) {

        size_t n = a.size(), m = b.size(), longestN = std::min(n, m); 
        std::vector<std::vector<std::vector<T>>> mat (n, std::vector<std::vector<T>>(m));
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {    // O(n * m)
                if (j != 0)
                    mat[i][j] = mat[i][j - 1];
                if (a[i] == b[j]) 
                    mat[i][j].push_back(a[i]);
                if (i != 0 && mat[i - 1][j].size() > mat[i][j].size()) 
                    mat[i][j] = std::move(mat[i - 1][j]);
            }
        }

        return mat[n - 1][m - 1];
    }

    inline std::string longest_common_subsequence(const std::string& a, const std::string& b) {
        std::vector<char> av(a.begin(), a.end());
        std::vector<char> bv(b.begin(), b.end());
        auto r = longest_common_subsequence(av, bv);
        return std::string(r.begin(), r.end());
    }
}

#endif

