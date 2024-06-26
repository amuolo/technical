#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Algorithms

#include "../utils/Algorithms.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(sort_index) {

	std::vector<int> a = { 4, 5, 1, 1, -2 };

	std::vector<size_t> r = { 4, 2, 3, 0, 1 };

	BOOST_TEST(tech::algorithms::sort_index(a) == r);
}

BOOST_AUTO_TEST_CASE(sort_index_worse) {

	//                     0  1  2  3   4  5   6   7  8  9 10 11 12 13 14 15 16
	std::vector<int> a = { 4, 5, 1, 1, -2, 4, -1, -1, 9, 8, 6, 2, 4, 3, 6, 5, 4 };

	std::vector<size_t> r = { 4, 6, 7, 2, 3, 11, 13, 0, 5, 12, 16, 1, 15, 10, 14, 9, 8 };

	BOOST_TEST(tech::algorithms::sort_index(a) == r);
}

BOOST_AUTO_TEST_CASE(sort_linear_counting_highmem) {

	std::vector<int> a = { 4, 5, 1, 1, -2, 4, -1, -1, 9, 8, 6, 2, 4, 3, 6, 5, 4 };

	std::vector<int> ar = { -2, -1, -1, 1, 1, 2, 3, 4, 4, 4, 4, 5, 5, 6, 6, 8, 9 };

	const auto& az = tech::algorithms::sort_linear_counting_highmem(a);

	BOOST_TEST(az == ar);

	std::vector<int> b = { 4, 5, 1, 1, 2, 4, 1, 0, 9, 8, 6, 2, 4, 3, 6, 5, 4 };

	std::vector<int> br = { 0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 6, 6, 8, 9 };

	const auto& bz = tech::algorithms::sort_linear_counting_highmem(b);

	BOOST_TEST(bz == br);
}

BOOST_AUTO_TEST_CASE(sort_nlogn_counting) {

	std::vector<int> a = { 4, 5, 1, 1, -2, 4, -1, -1, 9, 8, 6, 2, 4, 3, 6, 5, 4 };

	std::vector<int> ar = { -2, -1, -1, 1, 1, 2, 3, 4, 4, 4, 4, 5, 5, 6, 6, 8, 9 };

	const auto& az = tech::algorithms::sort_nlogn_counting(a);

	BOOST_TEST(az == ar);

	std::vector<int> b = { 4, 5, 1, 1, 2, 4, 1, 0, 9, 8, 6, 2, 4, 3, 6, 5, 4 };

	std::vector<int> br = { 0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 6, 6, 8, 9 };

	const auto& bz = tech::algorithms::sort_nlogn_counting(b);

	BOOST_TEST(bz == br);
}



/****************/
/*   SEQUENCE   */
/****************/

BOOST_AUTO_TEST_CASE(longest_common_subsequence_1) {

	std::string a1 = "afgh";
	std::string b1 = "fgahlm";
	std::string r1 = tech::algorithms::longest_common_subsequence(a1, b1);

	BOOST_TEST(r1 == "fgh");

	std::string a2 = "AquaVitae";
	std::string b2 = "AruTaVae";
	std::string r2 = tech::algorithms::longest_common_subsequence(a2, b2);

	BOOST_TEST(r2 == "AuaVae");

	std::string a3 = "betterte";
	std::string b3 = "ter";
	std::string r3 = tech::algorithms::longest_common_subsequence(a3, b3);

	BOOST_TEST(r3 == "ter");

	std::string a4 = "faPkdmElxiyjROOhuFHbaKpmmVAXQcPTiwtCiJC";
	std::string b4 = "BVObzwbRESpqhpvjiDGmlVJ";
	std::string r4 = tech::algorithms::longest_common_subsequence(a4, b4);

	BOOST_TEST(r4 == "ObpmVJ");

	std::string a5 = "QivcjGDoqoZKuiDcFHqRBdtuPRrisYZoSyQuERxvPiYZNfbWTZHqwLiCeebfYSAgSZiOc";
	std::string b5 = "OAAeqEXEmijuQqDEDYBlklSWURxcLxHlFBYuWSWMYHWJVHXigdjohAundoufZ";
	std::string r5 = tech::algorithms::longest_common_subsequence(a5, b5);

	BOOST_TEST(r5 == "ijqDcFBuSYWHifZ");
}

