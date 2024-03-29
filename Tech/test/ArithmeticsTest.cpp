#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE Arithmetics

#include "../utils/StandardLibs.hpp"
#include "../utils/Arithmetics.hpp"

#include <boost/test/included/unit_test.hpp>

using cxint = std::complex<int>;
using veccxint = std::vector<cxint>;

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
	
	veccxint a = { cxint(1, 2), cxint(2, -4) };
	veccxint b = { cxint(1, -2), cxint(-5, 2) };

	auto r1 = a + b;
	auto r2 = a - b;
	a += b;
	b -= a;

	BOOST_TEST(r1 == veccxint({ cxint(2, 0), cxint(-3, -2) }));
	BOOST_TEST(r2 == veccxint({ cxint(0, 4), cxint(7, -6) }));
	BOOST_TEST(a == r1);
	BOOST_TEST(b == -(r1 + r2) / 2);
}

BOOST_AUTO_TEST_CASE(vector_dotproduct) {

	veccxint a = { cxint(1, 2), cxint(2, -4) };
	veccxint b = { cxint(1, -2), cxint(-5, 2) };

	auto r1 = a * b;
	auto r2 = a * 2;
	auto r3 = 2 * a;

	BOOST_TEST(r1 == cxint(-21, -20));
	BOOST_TEST(r2 == veccxint({ cxint(2, 4), cxint(4, -8) }));
	BOOST_TEST(r3 == veccxint({ cxint(2, 4), cxint(4, -8) }));
	BOOST_TEST(r3 / 2 == a);
}

BOOST_AUTO_TEST_CASE(vector_kronecker_product) {

	veccxint a = { cxint(1, 2), cxint(2, -4) };
	veccxint b = { cxint(1, -2), cxint(-5, -2) };

	auto r1 = kronecker(a, b);

	BOOST_TEST(r1 == veccxint({ cxint(5, 0), cxint(-1, -12), cxint(-6, -8), cxint(-18, 16) }));
}

