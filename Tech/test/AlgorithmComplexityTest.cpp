#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE AlgorithmComplexity

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/AlgorithmComplexity.h"

#include <boost/test/included/unit_test.hpp>
#include <boost/algorithm/string.hpp>

using ld = long double;
using ull = unsigned long long;

BOOST_AUTO_TEST_CASE(constant_complexity_test) {

	ull i = 0;
	auto x = std::vector<ull>();

	auto study = tech::algorithm_complexity(
		[&]() { x.push_back(++i); },
		[&]() { x.push_back(++i); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(1)");
}

BOOST_AUTO_TEST_CASE(logarithmic_complexity_test) {

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < std::log((ld)n); i++)
			x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(boost::algorithm::contains(result, "log"));
}

BOOST_AUTO_TEST_CASE(n_complexity_test) {

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < n; i++)
			x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(n)");
}

BOOST_AUTO_TEST_CASE(n2_complexity_test) {

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < n; i++)
			for (ull i = 0; i < n; i++)
				x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(n^2)");
}

BOOST_AUTO_TEST_CASE(n3_complexity_test) {

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < n; i++)
			for (ull i = 0; i < n; i++)
				for (ull i = 0; i < n; i++)
					x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(n^3)");
}

BOOST_AUTO_TEST_CASE(n4_complexity_test) {

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < n; i++)
			for (ull i = 0; i < n; i++)
				for (ull i = 0; i < n; i++)
					for (ull i = 0; i < n; i++)
						x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(n^4)");
}