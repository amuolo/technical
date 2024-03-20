#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_
#define INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_

#include "StandardLibs.h"
#include "RandomMachine.h"
#include "Resources.h"

namespace tech
{
	struct algo_info {
		using ld = long double;
		using vec_ld = std::vector<ld>;

	public:
		std::function<ld(ld)> reducer;
		vec_ld timings;
		vec_ld slopes;

		algo_info() {}
		algo_info(std::function<ld(ld)> f) : reducer(f), timings(vec_ld()), slopes(vec_ld()) {}
	};

	class algorithm_complexity {
		using ld = long double;
		using vec_ld = std::vector<ld>;

		friend static class AlgorithmComplexityTest;

	private:
		std::function<void()> m_function;
		std::function<void()> m_increase_n;
		std::function<long long()> m_get_n;
		std::function<void()> m_reset;
		std::function<long long()> m_eval_benchmark;

		std::unordered_map<std::string, algo_info> m_slopes;

		std::map<long, long long> m_benchmark;
		std::string m_result;
		std::string m_status;

		short m_type;
		long long m_optimal_starting_n;
		long long m_optimal_num_it;
		long long m_timeout = (long long)(10000000);
		long long m_timein = (long long)(1000);

		void init_parameters() {
			auto n0 = m_get_n();
			m_increase_n();
			if (m_get_n() == n0)
				throw std::exception("algorithm_complexity: increase_n function provided is not working.");

			auto n1 = m_get_n();
			m_function();
			auto n2 = m_get_n();
			m_type = n2 - n1;

			m_reset();
			if (m_get_n() != 0)
				throw std::exception("algorithm_complexity: reset function not working properly.");
		};

		void init_slopes() {
			m_slopes.clear();
			m_slopes = {
				{ "O(log2(n))",  algo_info([&](ld x) { return std::pow(2., x / m_timein); }) },
				{ "O(log3(n))",  algo_info([&](ld x) { return std::pow(3., x / m_timein); }) },
				{ "O(log4(n))",  algo_info([&](ld x) { return std::pow(4., x / m_timein); }) },
				{ "O(log5(n))",  algo_info([&](ld x) { return std::pow(5., x / m_timein); }) },
				{ "O(log6(n))",  algo_info([&](ld x) { return std::pow(6., x / m_timein); }) },
				{ "O(log7(n))",  algo_info([&](ld x) { return std::pow(7., x / m_timein); }) },
				{ "O(log8(n))",  algo_info([&](ld x) { return std::pow(8., x / m_timein); }) },
				{ "O(log9(n))",  algo_info([&](ld x) { return std::pow(9., x / m_timein); }) },
				{ "O(log10(n))", algo_info([&](ld x) { return std::pow(10., x / m_timein); }) },

				{ "O(n)",      algo_info([&](ld x) { return x; }) },
				{ "O(n^2)",    algo_info([&](ld x) { return std::pow(x, 1./2.); }) },
				{ "O(n^3)",    algo_info([&](ld x) { return std::pow(x, 1./3.); }) },
				{ "O(n^4)",    algo_info([&](ld x) { return std::pow(x, 1./4.); }) },
				{ "O(n^5)",    algo_info([&](ld x) { return std::pow(x, 1./5.); }) },
				{ "O(n^6)",    algo_info([&](ld x) { return std::pow(x, 1./6.); }) },
				{ "O(n^7)",    algo_info([&](ld x) { return std::pow(x, 1./7.); }) },
				{ "O(n^8)",    algo_info([&](ld x) { return std::pow(x, 1./8.); }) },
				{ "O(n^9)",    algo_info([&](ld x) { return std::pow(x, 1./9.); }) },
				{ "O(n^10)",   algo_info([&](ld x) { return std::pow(x, 1./10.); }) },
				{ "O(n^11)",   algo_info([&](ld x) { return std::pow(x, 1./11.); }) },
				{ "O(n^12)",   algo_info([&](ld x) { return std::pow(x, 1./12.); }) },

				{ "O(e^n)",  algo_info([&](ld x) { return std::log(x); }) },
				{ "O(n!)",   algo_info([&](ld x) { return x; }) }  // TODO: not implemented yet
			};
		}

		void calibrate() {
			m_reset();
			m_optimal_num_it = 1;
			int checks = 0;

			// This will continue until the time needed is greater than 100 us for 5 times in a row
			while (checks < 5) {
				m_increase_n();
				m_optimal_starting_n = m_get_n();

				// Increase the number of iterations cautiously for increasing and decreasing algorithms
				if (m_type == 0 || (m_optimal_num_it < m_optimal_starting_n / 200)) {
					m_optimal_num_it++;
				}

				// Increasing n to compensate for the algorithm
				if (m_type < 0) {
					for (size_t i = 0; i < 2 * m_optimal_num_it + 1; i++)
						m_increase_n();
				}

				m_eval_benchmark = [&]() {
					auto t1 = std::chrono::high_resolution_clock::now();
					for (size_t i = 0; i < m_optimal_num_it; i++) {
						m_function();
					}
					auto t2 = std::chrono::high_resolution_clock::now();
					return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
					};

				long long time = m_eval_benchmark();
				if (time > m_timein) checks++;
				else			       checks = 0;
			}
		};

		void eval_slopes() {
			for (auto& [complexity, item] : m_slopes) {
				long n0 = 0;
				ld t0 = 0;
				for (const auto& [n1, t1] : m_benchmark) {
					auto funt1 = item.reducer(ld(t1));
					item.timings.push_back(funt1);
					if (n0 != 0) item.slopes.push_back(ld(funt1 - t0) / ld(n1 - n0));
					n0 = n1;
					t0 = funt1;
				}
			}
		}

		void eval_results() {
			if (!m_slopes.contains("O(n)")) throw std::exception("failed to retrieve slopes.");

			auto vt = std::views::values(m_benchmark);
			std::vector<long long> timings{ vt.begin(), vt.end() };

			auto getMinMaxSumAvg = [](const auto& v, const std::string& name) {
				if (v.size() == 0) throw std::exception((std::string("eval_results is trying to access empty storage ") + name).c_str());
				ld min = *std::max_element(v.begin(), v.end(), [](const ld d1, const ld d2) { return std::fabs(d1) > std::fabs(d2); });
				ld max = *std::max_element(v.begin(), v.end(), [](const ld d1, const ld d2) { return std::fabs(d1) < std::fabs(d2); });
				ld sum = std::accumulate(v.begin(), v.end(), ld(0), [](const ld r, const ld d) -> ld { return r + std::fabs(d); });
				ld avg = sum / v.size();
				bool valid = !(std::isnan(min) || std::isnan(max) || std::isnan(sum) || std::isnan(avg));
				return std::tuple(valid, min, max, sum, avg);
			};

			auto [valid, minTime, maxTime, sumTime, avgTime] = getMinMaxSumAvg(timings, "O(1)");

			if (valid && std::find_if(timings.begin(), timings.end(), [&](ld x) { return x - avgTime > minTime; }) == timings.end()) {
				m_result = "O(1)";
				return;
			}

			for (const auto& [complexity, item] : m_slopes) {
				auto slopes = item.slopes;

				auto [valid, minSlope, maxSlope, sumSlope, avgSlope] = getMinMaxSumAvg(slopes, complexity);

				if (valid && std::find_if(slopes.begin(), slopes.end(), [&](ld x) { return std::fabs(std::fabs(x) - avgSlope) > minSlope; }) == slopes.end()) {
					m_result = complexity;
					return;
				}
			}

			m_result = "undefined";
		}

	public:
		algorithm_complexity(
			const std::function<void()>& fun,
			const std::function<void()>& increase_n,
			const std::function<long long()>& get_n,
			const std::function<void()>& reset)
			: m_function(fun), m_increase_n(increase_n), m_get_n(get_n), m_reset(reset), m_result("undefined"), m_status(""),
			m_optimal_num_it(1), m_optimal_starting_n(0), m_type(0)
		{};

		void run_analysis() {
			try {
				m_benchmark.clear();
				init_parameters();
				init_slopes();
				calibrate();

				long long time = 0;
				auto t0 = std::chrono::high_resolution_clock::now();

				while (m_benchmark.size() < 10) {
					long n = m_get_n();
					m_benchmark.insert(std::make_pair(m_get_n(), m_eval_benchmark()));
					auto t = std::chrono::high_resolution_clock::now();

					if (std::chrono::duration_cast<std::chrono::microseconds>(t - t0).count() > m_timeout) {
						m_status = "max time exceeded";
						break;
					}

					if (get_used_memory_percentage() > 95) {
						m_status = "max memory exceeded";
						break;
					}

					while (m_get_n() < n + n)
						m_increase_n();
				}
				eval_slopes();
				eval_results();
			}
			catch (std::exception e) {
				std::cout << "\n\n Error: " << e.what() << std::endl << std::endl;
			}
		};

		void print_benchmark() {
			print(m_benchmark);
		}

		void print_timings(std::string name) {
			if (m_slopes.contains(name))
				print(m_slopes[name].timings);
			else
				std::cout << "timings not found";
		}

		void print_slopes(std::string name) {
			if (m_slopes.contains(name))
				print(m_slopes[name].slopes);
			else
				std::cout << "slopes not found";
		}

		std::string get_result() {
			return m_result;
		}

		std::string get_status() {
			return m_status;
		}
	};
}

#endif

