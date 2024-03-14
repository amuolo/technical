#include "utils/StandardLibs.h"
#include "utils/InputOutput.h"

#include "utils/AlgorithmComplexity.h"

#include "test/Tests.h"

int main()
{
	auto random = random_machine<int>();

	auto x = std::vector<int>();

	auto complexity = algorithm_complexity<int>(
		[&]() { x.push_back(random.get(1, 10)); },
		[&]() { return x.size(); },
		[&]() { x.clear(); });

	std::cout << std::endl << complexity.analysis() << std::endl << std::endl;

	complexity.print_benchmark();

	std::cout << std::endl << std::endl;




	auto result = run_all_tests();

	std::cout << std::endl << std::endl;
	
	print(result);

	std::cout << std::endl << std::endl;

	std::cin.get();

	return 0;
}
