#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE AlgorithmComplexity

#include "../utils/StandardLibs.h"
#include "../utils/InputOutput.h"
#include "../utils/AlgorithmComplexity.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(linear_complexity_test) {

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

	std::vector<long long> keys;

	auto study = tech::algorithm_complexity(
		[&]() { x[keys.at(random.get(0, keys.size() -1))] = num(); },
		[&]() { long long n; do { n = num(); } while (x.contains(n)); keys.push_back(n); x[n] = num(); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	auto result = study.get_result();

	BOOST_TEST(result == "O(log(n))");
}