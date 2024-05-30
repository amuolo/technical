#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_
#define INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_

#include "StandardLibs.hpp"
#include "RandomMachine.hpp"
#include "Resources.hpp"
#include "InputOutput.hpp"

#include <boost/algorithm/string.hpp>

namespace tech
{
	struct algo_info {
		using l = long;
		using ld = long double;
		using vec_ld = std::vector<ld>;

	public:
		std::function<ld(l, ld)> reducer;
		vec_ld timings;
		vec_ld slopes;
		ld error;

		algo_info() {}

		algo_info(std::function<ld (l, ld)> f) 
			: reducer(f), timings(vec_ld()), slopes(vec_ld()), error(std::numeric_limits<ld>::max()) {}
	};

	class algorithm_complexity {
		using l = long;
		using ll = long long;
		using ld = long double;
		using vec_ld = std::vector<ld>;

		friend static class AlgorithmComplexityTest;

	private:
		std::function<void()> m_function;
		std::function<void()> m_increase_n;
		std::function<ll()> m_get_n;
		std::function<void()> m_reset;
		std::function<ll()> m_eval_benchmark;

		std::unordered_map<std::string, algo_info> m_slopes;

		std::map<l, ll> m_benchmark;
		std::string m_result;
		std::string m_status;

		short m_type;
		ll m_optimal_starting_n;
		ll m_optimal_num_it;
		ll m_timeOut = (ll)(10000000);
		ll m_timeMin = (ll)(1000000);
		ll m_timeIn = (ll)(100);

		std::function<std::chrono::steady_clock::time_point()> m_get_time_now 
			= []() { return std::chrono::high_resolution_clock::now(); };

		std::function<ll(std::chrono::steady_clock::time_point)> m_get_time_elapsed 
			= [&](std::chrono::steady_clock::time_point t0) { return std::chrono::duration_cast<std::chrono::microseconds>(m_get_time_now() - t0).count(); };

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

		bool is_super_complex(std::string result) {
			std::vector<std::string> items = { "O(n^10)", "O(n^11)", "O(n^12)", "O(2^n)", "O(3^n)", "O(4^n)" };
			return std::any_of(items.begin(), items.end(), [&](const auto& x) { return x == result; });
		}

		bool is_complex(std::string result) {
			std::vector<std::string> items = { "O(n^2)", "O(n^3)", "O(n^4)", "O(n^5)", "O(n^6)", "O(n^7)", "O(n^8)", "O(n^9)" };
			return std::any_of(items.begin(), items.end(), [&](const auto& x) { return x == result; });
		}

		void init_slopes() {
			m_slopes.clear();
			m_slopes = {
				{ "O(log2(n))",  algo_info([&](l n, ld x) { return std::pow(2., x / m_timeIn); }) },
				{ "O(log3(n))",  algo_info([&](l n, ld x) { return std::pow(3., x / m_timeIn); }) },
				{ "O(log4(n))",  algo_info([&](l n, ld x) { return std::pow(4., x / m_timeIn); }) },

				{ "O(1)",      algo_info([](l n, ld x) { return x; }) },
				{ "O(n)",      algo_info([](l n, ld x) { return x; }) },
				{ "O(n^2)",    algo_info([](l n, ld x) { return std::pow(x, 1./2.); }) },
				{ "O(n^3)",    algo_info([](l n, ld x) { return std::pow(x, 1./3.); }) },
				{ "O(n^4)",    algo_info([](l n, ld x) { return std::pow(x, 1./4.); }) },
				{ "O(n^5)",    algo_info([](l n, ld x) { return std::pow(x, 1./5.); }) },
				{ "O(n^6)",    algo_info([](l n, ld x) { return std::pow(x, 1./6.); }) },
				{ "O(n^7)",    algo_info([](l n, ld x) { return std::pow(x, 1./7.); }) },
				{ "O(n^8)",    algo_info([](l n, ld x) { return std::pow(x, 1./8.); }) },
				{ "O(n^9)",    algo_info([](l n, ld x) { return std::pow(x, 1./9.); }) },
				{ "O(n^10)",   algo_info([](l n, ld x) { return std::pow(x, 1./10.); }) },
				{ "O(n^11)",   algo_info([](l n, ld x) { return std::pow(x, 1./11.); }) },
				{ "O(n^12)",   algo_info([](l n, ld x) { return std::pow(x, 1./12.); }) },
								
				{ "O(2^n)",  algo_info([](l n, ld x) { return std::log2(x); }) },
				{ "O(3^n)",  algo_info([](l n, ld x) { return std::log2(x) / std::log2(ld(3)); }) },
				{ "O(4^n)",  algo_info([](l n, ld x) { return std::log2(x) / std::log2(ld(4)); }) },
			};
		}

		void calibrate() {
			m_reset();
			m_optimal_num_it = 1;
			int checks = 0;

			// This will continue until the time needed is greater than 100 us for 5 times in a row
			while (checks < 5) {
				if(checks == 0) m_increase_n();
				m_optimal_starting_n = m_get_n();

				// Increase the number of iterations cautiously for increasing and decreasing algorithms
				if (checks == 0 && (m_type == 0 || (m_optimal_num_it < m_optimal_starting_n / 200))) {
					m_optimal_num_it++;
				}

				// Increasing n to compensate for the algorithm
				if (m_type < 0) {
					for (size_t i = 0; i < (-m_type) * m_optimal_num_it; i++)
						m_increase_n();
				}

				// Build benchmark function
				m_eval_benchmark = [&]() {
					auto t1 = m_get_time_now();
					for (size_t i = 0; i < m_optimal_num_it; i++) {
						m_function();
					}
					auto t2 = m_get_time_now();
					return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
					};

				if (m_eval_benchmark() > m_timeIn) checks++;
				else checks = 0;
			}
		};

		void eval_slopes() {
			init_slopes();
			for (auto& [complexity, item] : m_slopes) {
				l n0 = 0;
				ld t0 = 0;
				for (const auto& [n1, t1] : m_benchmark) {
					auto funt1 = item.reducer(n1, ld(t1));
					item.timings.push_back(funt1);
					if (n0 != 0) item.slopes.push_back(ld(funt1 - t0) / ld(n1 - n0));
					n0 = n1;
					t0 = funt1;
				}
			}
		}

		void eval_results() {
			if (!m_slopes.contains("O(n)")) throw std::exception("failed to retrieve slopes.");

			auto getMinMaxSumAvg = [](const auto& v, const std::string& name) {
				if (v.size() <= 1) throw std::exception((std::string("eval_results is trying to access empty benchmarks ") + name).c_str());
				ld min = *std::max_element(v.begin(), v.end(), [](const ld d1, const ld d2) { return std::fabs(d1) > std::fabs(d2); });
				ld max = *std::max_element(v.begin(), v.end(), [](const ld d1, const ld d2) { return std::fabs(d1) < std::fabs(d2); });
				ld sum = std::accumulate(v.begin(), v.end(), ld(0), [](const ld r, const ld d) -> ld { return r + std::fabs(d); });
				ld avg = sum / v.size();
				bool valid = !(std::isnan(min) || std::isnan(max) || std::isnan(sum) || std::isnan(avg));
				return std::tuple(valid, min, max, sum, avg);
			};

			auto [valid, minSlopeBench, maxSlopeBench, sumSlopeBench, avgSlopeBench] = getMinMaxSumAvg(get_slopes("O(n)"), "O(n)");

			for (auto& [complexity, info] : m_slopes) {
				auto [valid, minSlope, maxSlope, sumSlope, avgSlope] = getMinMaxSumAvg(info.slopes, complexity);

				if (complexity == "O(1)") {
					auto times = info.timings;
					auto minN = (m_benchmark.begin()->first + std::next(m_benchmark.begin())->first) / 2.;
					auto maxN = (std::prev(m_benchmark.end())->first + std::prev(std::prev(m_benchmark.end()))->first) / 2.;
					auto growth = std::log(maxN) / std::log(minN);
					auto tolerance = std::min(std::max(growth * 0.7, 1.), 1.8);

					if (valid && (times.at(0) + times.at(1)) * tolerance / 2. > (times.at(times.size() - 2) + times.back()) / 2.) {
						info.error = 0;
						break;
					}
				}		
				else if (valid) {
					if (complexity == "O(1)" || boost::algorithm::contains(complexity, "log") || avgSlopeBench > 0.9)
						info.error = std::fabs((maxSlope - minSlope) / avgSlope);
				}
			}

			m_result = std::min_element(m_slopes.begin(), m_slopes.end(), [](const auto& a, const auto& b) { return a.second.error < b.second.error; })->first;
		}

	public:
		algorithm_complexity(
			const std::function<void()>& fun,
			const std::function<void()>& increase_n,
			const std::function<ll()>& get_n,
			const std::function<void()>& reset)
			: m_function(fun), m_increase_n(increase_n), m_get_n(get_n), m_reset(reset), m_result("undefined"), m_status(""),
			m_optimal_num_it(1), m_optimal_starting_n(0), m_type(0)
		{};

		void run_analysis() {
			try {
				m_benchmark.clear();
				init_parameters();
				calibrate();
				auto t0 = m_get_time_now();

				while (m_benchmark.size() < 20 || m_get_time_elapsed(t0) < m_timeMin) {
					l n = m_get_n();
					m_benchmark.insert(std::make_pair(m_get_n(), m_eval_benchmark()));

					if (m_get_time_elapsed(t0) > m_timeOut) {
						m_status = "max time exceeded";
						break;
					}

					if (resources::get_used_memory_percentage() > 95) {
						m_status = "max memory exceeded";
						break;
					}

					eval_slopes();
					auto newN = n + std::max(l(1), n / 2);

					if (m_benchmark.size() > 3) {
						eval_results();
						if (is_super_complex(m_result))
							newN = n + 1;
						else if (is_complex(m_result))
							newN = n + std::max(l(1), n / 3);
					}

					while (m_get_n() < newN)
						m_increase_n();
				}
			}
			catch (std::exception e) {
				std::cout << "\n\n Error: " << e.what() << std::endl << std::endl;
			}
		};

		void print_benchmark() {
			print(m_benchmark);
		}

		vec_ld get_timings(std::string name) {
			if (m_slopes.contains(name))
				return m_slopes[name].timings;
			else
				return vec_ld();
		}

		vec_ld get_slopes(std::string name) {
			if (m_slopes.contains(name))
				return m_slopes[name].slopes;
			else
				return vec_ld();
		}

		void print_errors() {
			for (const auto& [complexity, info] : m_slopes) {
				std::cout << " " << complexity << ": " << info.error << std::endl;
			}
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

