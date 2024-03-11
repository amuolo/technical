// Technical.cpp : Defines the entry point for the application.
//

#include "utils/StandardLibs.h"
#include "utils/IntervalMap.h"
#include "utils/InputOutput.h"

#include "test/Tests.h"

using namespace std;

int main()
{
	run_all_tests();

	std::cout << std::endl << std::endl;

	cin.get();

	return 0;
}
