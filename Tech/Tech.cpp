#include "utils/StandardLibs.h"
#include "utils/InputOutput.h"

#include "utils/AlgorithmComplexity.h"

//#include "test/Tests.h"

#include <boost/regex.h>

int main()
{
	auto random = random_machine<int>();

	auto x = std::vector<int>();

	auto study = algorithm_complexity<int>(
		[&]() { x.push_back(random.get(1, 10)); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	study.run_analysis();

	std::cout << std::endl << study.get_result() << std::endl << std::endl;

	study.print_benchmark();

	std::cout << std::endl << std::endl;

	study.print_slopes();



	//run_all_tests();

	std::cout << std::endl << std::endl;
	
	std::cin.get();

	return 0;
}
