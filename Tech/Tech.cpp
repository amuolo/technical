﻿#include "utils/StandardLibs.hpp"
#include "utils/InputOutput.hpp"

#include "utils/Complexity.hpp"

int main()
{
	std::cout << std::endl << "Start" << std::endl;

	using ld = long double;
	using ull = unsigned long long;

	std::complex<ld> x = 1234;
	ull n = 1;

	auto fun = [&]() {
		for (ull i = 0; i < std::pow((ld)2., (ld)n); i++)
			x = x * std::exp(std::complex<ld>(0.0, 1.0) * (ld)std::acos(-1) / (ld)1234.);
		};

	auto study = tech::algorithm_complexity(
		[&]() { fun(); },
		[&]() { n++; },
		[&]() { return n; },
		[&]() { n = 0; });

	ull attempt = 0;
	do {
		std::cout << ++attempt << std::endl;
		study.run_analysis();
	} while (boost::algorithm::contains(study.get_result(), "^n"));
	//while (study.get_result() == "O(n^2)");
	//while (study.get_result() == "O(1)");
	//while (boost::algorithm::contains(study.get_result(), "log"))
	//while ((study.get_result() == "O(1)" || boost::algorithm::contains(study.get_result(), "O(log")));

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
