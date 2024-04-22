#include "utils/StandardLibs.hpp"
#include "utils/InputOutput.hpp"

#include "utils/AlgorithmComplexity.hpp"

int main()
{
	std::cout << std::endl << "Start" << std::endl;

	using ld = long double;
	using ull = unsigned long long;

	ull i = 0;
	auto x = std::vector<ull>();
	x.reserve(size_t(100000000));

	auto study = tech::algorithm_complexity(
		[&]() { x.push_back(++i); },
		[&]() { x.push_back(++i); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	ull attempt = 0;
	do {
		attempt++;
		study.run_analysis();
	} 
	//while (study.get_result() == "O(n^2)");
	//while (study.get_result() == "O(1)");
	//while (boost::algorithm::contains(study.get_result(), "log"))
	while ((study.get_result() == "O(1)" || boost::algorithm::contains(study.get_result(), "O(log")));

	std::cout << std::endl << attempt << "   " << study.get_result() << std::endl << std::endl;

	study.print_benchmark();

	std::cout << std::endl << std::endl;

	tech::print(study.get_slopes("O(n)"));

	std::cout << std::endl << std::endl << std::endl;



	tech::print(study.get_timings("O(log10(n))"));

	std::cout << std::endl << std::endl;

	tech::print(study.get_slopes("O(log10(n))"));

	std::cout << std::endl << std::endl << std::endl;



	tech::print(study.get_timings("O(e^n)"));

	std::cout << std::endl << std::endl;

	tech::print(study.get_slopes("O(e^n)"));

	std::cout << std::endl << std::endl << std::endl;



	study.print_errors();


	std::cout << std::endl << std::endl;
	//std::cin.get();
	return 0;
}
