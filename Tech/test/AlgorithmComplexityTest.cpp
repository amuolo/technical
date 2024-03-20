#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE AlgorithmComplexity

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/AlgorithmComplexity.h"

#include <boost/test/included/unit_test.hpp>

using ld = long double;
using ull = unsigned long long;

BOOST_AUTO_TEST_CASE(constant_complexity_test) {

	auto random = tech::random_machine<int>();

	auto x = std::vector<int>();

	auto study = tech::algorithm_complexity(
		[&]() { x.push_back(random.get(1, 10)); },
		[&]() { x.push_back(random.get(1, 100)); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(1)");
}

BOOST_AUTO_TEST_CASE(logarithmic_complexity_test) {

	auto random = tech::random_machine<long long>();
	auto num = [&]() { return random.get(std::numeric_limits<long long>::min(), std::numeric_limits<long long>::max()); };

	auto x = std::map<long long, long long>();
	auto keys = std::vector<long long>();
	keys.reserve(1000000000);

	auto study = tech::algorithm_complexity(
		[&]() { x[keys.at(random.get(0, keys.size() -1))] = num(); },
		[&]() { long long n; do { n = num(); } while (x.contains(n)); keys.push_back(n); x[n] = num(); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(log(n))");
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