#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_
#define INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_

#include "StandardLibs.h"
#include "RandomMachine.h"
#include "Resources.h"

namespace tech
{
	struct algorithm_complexity_info {
		using ld = long double;
		using vec_ld = std::vector<ld>;
		std::function<ld(ld)> reducer;
		vec_ld timings;
		vec_ld slopes;
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

		std::unordered_map<std::string, algorithm_complexity_info> m_slopes;

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
				{ "O(log2(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log3(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log4(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log5(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log6(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log7(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log8(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log9(n))",  { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },
				{ "O(log10(n))", { [&](ld x) { return std::pow(2., x / m_timein); }, vec_ld(), vec_ld() } },

				{ "O(n)",        { [&](ld x) { return x; },                vec_ld(), vec_ld() } },
				{ "O(n^2)",      { [&](ld x) { return std::pow(x, -2); },  vec_ld(), vec_ld() } },
				{ "O(n^3)",      { [&](ld x) { return std::pow(x, -3); },  vec_ld(), vec_ld() } },
				{ "O(n^4)",      { [&](ld x) { return std::pow(x, -4); },  vec_ld(), vec_ld() } },
				{ "O(n^5)",      { [&](ld x) { return std::pow(x, -5); },  vec_ld(), vec_ld() } },
				{ "O(n^6)",      { [&](ld x) { return std::pow(x, -6); },  vec_ld(), vec_ld() } },
				{ "O(n^7)",      { [&](ld x) { return std::pow(x, -7); },  vec_ld(), vec_ld() } },
				{ "O(n^8)",      { [&](ld x) { return std::pow(x, -8); },  vec_ld(), vec_ld() } },
				{ "O(n^9)",      { [&](ld x) { return std::pow(x, -9); },  vec_ld(), vec_ld() } },
				{ "O(n^10)",     { [&](ld x) { return std::pow(x, -10); }, vec_ld(), vec_ld() } },
				{ "O(n^11)",     { [&](ld x) { return std::pow(x, -11); }, vec_ld(), vec_ld() } },
				{ "O(n^12)",     { [&](ld x) { return std::pow(x, -12); }, vec_ld(), vec_ld() } },

				{ "O(e^n)",  { [&](ld x) { return std::log(x); },  vec_ld(), vec_ld() } },
				{ "O(n!)",   { [&](ld x) { return x; },            vec_ld(), vec_ld() } }  // TODO: not implemented yet
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
				auto min = *std::max_element(v.begin(), v.end(), [](const auto& d1, const auto& d2) { return std::fabs(d1) > std::fabs(d2); });
				auto max = *std::max_element(v.begin(), v.end(), [](const auto& d1, const auto& d2) { return std::fabs(d1) < std::fabs(d2); });
				auto sum = std::accumulate(v.begin(), v.end(), ld(0), [](const ld r, const auto& d) -> ld { return r + std::fabs(d); });
				auto avg = sum / v.size();
				return std::tuple(min, max, sum, avg);
			};
						
			auto [minTime, maxTime, sumTime, avgTime] = getMinMaxSumAvg(timings, "O(1)");

			if (std::find_if(timings.begin(), timings.end(), [&](ld x) { return x - avgTime > minTime; }) == timings.end()) {
				m_result = "O(1)";
				return;
			}

			for (const auto& [complexity, item] : m_slopes) {						
				auto [minSlope, maxSlope, sumSlope, avgSlope] = getMinMaxSumAvg(item.slopes, complexity);

				if (std::find_if(item.slopes.begin(), item.slopes.end(), [&](ld x) { return std::fabs(x) - avgSlope > minSlope; }) == item.slopes.end()) {
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

				while (m_benchmark.size() < 100) {
					long n = m_get_n();
					m_benchmark.insert(std::make_pair(m_get_n(), m_eval_benchmark()));

					if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count() > m_timeout) {
						m_status = "max time exceeded";
						break;
					}

					if (get_used_memory_percentage() > 95) {
						m_status = "max memory exceeded";
						break;
					}

					while (m_get_n() < n + m_optimal_starting_n)
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

