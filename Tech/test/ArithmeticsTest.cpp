#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Arithmetics

#include "../utils/StandardLibs.hpp"
#include "../utils/Arithmetics.hpp"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(pair_sum) {
	auto res1 = std::make_pair(2, 2) + std::make_pair(1, 3);

	BOOST_TEST(res1.first == 3);
	BOOST_TEST(res1.second == 5);

	auto res2 = std::make_pair(2.3, 'x') + std::make_pair(4.5, 'y');

	BOOST_TEST(res2.first == 6.8);
	BOOST_TEST(res2.second == -15);
}

BOOST_AUTO_TEST_CASE(tuple_sum) {
	auto res1 = std::tuple(2, 2.3, 'a') + std::tuple(3, 1.1, 'b');

	BOOST_TEST(std::get<0>(res1) == 5);
	BOOST_TEST(std::get<1>(res1) == 3.4);
	BOOST_TEST(std::get<2>(res1) == -61);

	std::string a = "hello", b = " ", c = "world", d = "!";
	auto res2 = std::tuple(a.data(), c.data()) + std::tuple(b.data(), d.data());
	auto str = std::string(std::get<0>(res2)) + std::string(std::get<1>(res2));

	BOOST_TEST(str == "hello world!");
}

BOOST_AUTO_TEST_CASE(vector_sum) {
	
	using cxint = std::complex<int>;

	std::vector<cxint> a = { cxint(1, 2), cxint(2, -4) };
	std::vector<cxint> b = { cxint(1, -2), cxint(-5, 2) };

	auto r = a * b;

	BOOST_TEST(r == cxint(-21, -20));
	BOOST_TEST(a * 2 == std::vector<cxint>({ cxint(2, 4), cxint(4, -8) }));
}

BOOST_AUTO_TEST_CASE(vector_dotproduct) {

	BOOST_TEST(2 == 2);
}

BOOST_AUTO_TEST_CASE(vector_kronecker_product) {

	BOOST_TEST(2 == 2);
}

