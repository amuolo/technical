#pragma once

#ifndef INCLUDE_UTILS_ARITHMETICS_H_
#define INCLUDE_UTILS_ARITHMETICS_H_

#include "StandardLibs.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>

template <class T1, class T2>
inline std::string tech_sum(const T1& a, const T2& b) requires(std::is_same_v<T1, char*>) 
{
	auto i = a ? std::string(a) : "";
	auto j = b ? std::string(b) : "";
	auto r = i + j;
	return r;
}

template <class T1, class T2>
inline T1 tech_sum(const T1& a, const T2& b) requires(!std::is_same_v<T1, char*> && !std::is_same_v<T2, char*>) 
{
	return a + b;
}

/**********************************************************/
/*                         Tuple                          */
/**********************************************************/

/***************/
/*     Sum     */
/***************/

template <class T1, class T2, class W1, class W2>
inline auto operator + (const std::pair<T1, T2>& a, const std::pair<W1, W2>& b) 
{
	return std::pair(tech_sum(a.first, b.first), 
					 tech_sum(a.second, b.second));
};

template <class T1, class T2, class W1, class W2>
inline auto operator + (const std::tuple<T1, T2>& a, const std::tuple<W1, W2>& b) 
{
	return std::tuple(tech_sum(std::get<0>(a), std::get<0>(b)), 
					  tech_sum(std::get<1>(a), std::get<1>(b)));
};

template <class T1, class T2, class T3, class W1, class W2, class W3>
inline auto operator + (const std::tuple<T1, T2, T3>& a, const std::tuple<W1, W2, W3>& b) 
{
	return std::tuple(tech_sum(std::get<0>(a), std::get<0>(b)), 
					  tech_sum(std::get<1>(a), std::get<1>(b)), 
					  tech_sum(std::get<2>(a), std::get<2>(b)));
}

/**********************************************************/
/*                     STL Containers                     */
/**********************************************************/

/***************/
/*   Product   */
/***************/

template <class T, class Q>
inline T operator * (const std::vector<T>& a, const std::vector<Q>& b)
{
	if (a.size() != b.size())
		throw std::exception("vector size mismatch");

	T r = boost::lexical_cast<T>("0.0");

#pragma omp declare reduction( + : T : omp_out=omp_out+omp_in) initializer(omp_priv(omp_orig))
#pragma omp declare reduction( + : std::complex<T> : omp_out=omp_out+omp_in) initializer(omp_priv(omp_orig))
#pragma omp simd reduction(+:r)
	for (size_t i = 0; i < a.size(); i++) {
		if constexpr (boost::is_complex<T>::value)
			r += std::conj(a[i]) * b[i];
		else
			r += a[i] * b[i];
	}
	
	return r;
}

template <class T, class Q>
std::vector<T> kronecker(const std::vector<T>& lhs, const std::vector<Q>& rhs)
{
	std::vector<T> r(lhs.size() * rhs.size(), boost::lexical_cast<T>("0.0"));
	
#pragma omp simd collapse(2)
	for (size_t i = 0; i < lhs.size(); i++)
		for (size_t j = 0; j < rhs.size(); j++)
			r[i * rhs.size() + j] = lhs[i] * rhs[j];

	return r;
}

template <class T, class S>
inline std::vector<T>& operator *= (std::vector<T>& v, const S& a)
{
#pragma omp simd
	for (size_t i = 0; i < v.size(); i++)
		v[i] *= a;
	return v;
}

template <class T, class S>
inline std::vector<T> operator * (std::vector<T> v, const S& a) 
{
	return v *= a;
}


template <class T, class S>
inline std::vector<T> operator * (const S& a, std::vector<T> v) 
{
	return v *= a;
}

/****************/
/*   Division   */
/****************/

template <class T>
inline std::vector<T>& operator /= (std::vector<T>& v, const T& a)
{
#pragma omp simd
	for (size_t i = 0; i < v.size(); i++)
		v[i] /= a;
	return v;
}


template <class T>
inline std::vector<T> operator / (std::vector<T> v, const T& a)
{
	return v /= a;
}

/***************/
/*     Sum     */
/***************/

template <class T>
inline std::vector<T>& operator += (std::vector<T>& lhs, const std::vector<T>& rhs) requires(std::is_arithmetic_v<T>)
{
	if (lhs.size() != rhs.size())
		throw std::exception("vector size mismatch");

#pragma omp simd
	for (size_t i = 0; i < lhs.size(); i++)
		lhs[i] += rhs[i];

	return lhs;
}


template <class T>
inline std::vector<T> operator + (std::vector<T> lhs, const std::vector<T>& rhs)
{
	return lhs += rhs;
}

/****************/
/*  Difference  */
/****************/

template <class T>
inline std::vector<T>& operator -= (std::vector<T>& lhs, const std::vector<T>& rhs) 
{
	if (lhs.size() != rhs.size())
		throw std::exception("vector size mismatch");

#pragma omp simd
	for (size_t i = 0; i < lhs.size(); i++)
		lhs[i] -= rhs[i];

	return lhs;
}


template <class T>
inline std::vector<T> operator - (std::vector<T> lhs, const std::vector<T>& rhs)
{
	return lhs -= rhs;
}


#endif

