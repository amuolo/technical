#include "utils/StandardLibs.h"
#include "utils/InputOutput.h"

#include "utils/AlgorithmComplexity.h"

//#include "test/Tests.h"

#include <boost/regex.h>

int main()
{
	auto random = tech::random_machine<long long>();
	auto num = [&]() { return random.get(std::numeric_limits<long long>::min(), std::numeric_limits<long long>::max()); };

	auto x = std::map<long long, long long>();

	std::vector<long long> keys;

	auto study = tech::algorithm_complexity(
		[&]() { x[keys.at(random.get(0, keys.size() - 1))] = num(); },
		[&]() { long long n; do { n = num(); } while (x.contains(n)); keys.push_back(n); x[n] = num(); },
		[&]() { return x.size(); },
		[&]() { x.clear(); keys.clear(); });

	study.run_analysis();

	std::cout << std::endl << study.get_result() << std::endl << std::endl;

	study.print_benchmark();

	std::cout << std::endl << std::endl;

	study.print_timings("O(n)");

	std::cout << std::endl << std::endl;

	study.print_slopes("O(n)");



	//run_all_tests();

	std::cout << std::endl << std::endl;
	
	std::cin.get();

	return 0;
}
