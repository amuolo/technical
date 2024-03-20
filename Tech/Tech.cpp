#include "utils/StandardLibs.h"
#include "utils/InputOutput.h"

#include "utils/AlgorithmComplexity.h"

//#include "test/Tests.h"

#include <boost/regex.h>

int main()
{
	using ld = long double;
	using ull = unsigned long long;

	ull i = 0;
	auto x = std::vector<ull>();

	auto study = tech::algorithm_complexity(
		[&]() { x.push_back(++i); },
		[&]() { x.push_back(++i); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	std::cout << std::endl << study.get_result() << std::endl << std::endl;

	study.print_benchmark();

	std::cout << std::endl << std::endl;

	study.print_slopes("O(n)");

	std::cout << std::endl << std::endl << std::endl;



	study.print_timings("O(log10(n))");

	std::cout << std::endl << std::endl;

	study.print_slopes("O(log10(n))");

	std::cout << std::endl << std::endl << std::endl;



	study.print_timings("O(e^n)");

	std::cout << std::endl << std::endl;

	study.print_slopes("O(e^n)");

	std::cout << std::endl << std::endl << std::endl;



	study.print_errors();


	std::cout << std::endl << std::endl;
	std::cin.get();
	return 0;
}
