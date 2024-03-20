#include "utils/StandardLibs.h"
#include "utils/InputOutput.h"

#include "utils/AlgorithmComplexity.h"

//#include "test/Tests.h"

#include <boost/regex.h>

int main()
{
	using ld = long double;
	using ull = unsigned long long;

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

	std::cout << std::endl << study.get_result() << std::endl << std::endl;

	study.print_benchmark();

	std::cout << std::endl << std::endl;

	study.print_slopes("O(n)");

	std::cout << std::endl << std::endl;

	study.print_timings("O(log2(n))");


	//run_all_tests();

	std::cout << std::endl << std::endl;
	
	std::cin.get();

	return 0;
}
