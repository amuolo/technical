#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_
#define INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_

#include "StandardLibs.h"
#include "RandomMachine.h"
#include "Resources.h"

template <typename TParams>
class algorithm_complexity {
	friend static class AlgorithmComplexityTest;

private:
	std::function<void()> m_function;
	std::function<void()> m_increase_n;
	std::function<long long()> m_get_n;
	std::function<void()> m_reset;
	std::function<long long()> m_eval;

	std::map<long, long long> m_benchmark;
	std::vector<double> m_slopes;
	std::string m_result;
	std::string m_status;

	short m_type;
	long long m_optimal_starting_n;
	long long m_optimal_num_it;

	long long m_tenSecs = (long long)(10000000);

	void init() {
		m_optimal_num_it = 1;
		m_optimal_starting_n = 0;

		auto n0 = m_get_n();
		m_increase_n();
		if (m_get_n() == n0)
			throw std::exception("algorithm_complexity: increase_n function provided is not working.");

		auto n1 = m_get_n();
		m_function();
		auto n2 = m_get_n();
		m_type = n2 - n0;

		m_reset();
		if (m_get_n() != 0)
			throw std::exception("algorithm_complexity: reset function not working properly.");
	};

	void calibrate() {
		m_reset();
		int checks = 0;
		long long time = 0;

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

			m_eval = [&]() {
				auto t1 = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < m_optimal_num_it; i++) {
					m_function();
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
			};

			time = m_eval();
			if (time > 100) checks++;
			else			checks = 0;
		}
	};

public:
	algorithm_complexity(
		const std::function<void()>& fun,
		const std::function<long long()> get_n,
		const std::function<void()> reset)
		: m_function(fun), m_increase_n(fun), m_get_n(get_n), m_reset(reset), m_result("undefined"), m_status("") {
		init();
	};

	algorithm_complexity(
		const std::function<void()>& fun,
		const std::function<void()>& increase_n,
		const std::function<long long()>& get_n,
		const std::function<void()>& reset)
		: m_function(fun), m_increase_n(increase_n), m_get_n(get_n), m_reset(reset), m_result("undefined"), m_status("") {
		init();
	};

	void eval_benchmark_slopes() {
		m_slopes.clear();
		long prevKey = 0;
		long long prevVal = 0;
		for (const auto& [key, value] : m_benchmark) {
			if (prevKey != 0)
				m_slopes.push_back((value - prevVal) / double(key - prevKey));
			prevKey = key;
			prevVal = value;
		}
	}

	void eval_results() {
		auto sum = std::reduce(m_slopes.begin(), m_slopes.end());
		auto max = *std::max_element(m_slopes.begin(), m_slopes.end());
		if (sum < max / 100)
			m_result = "O(1)";
		else
			m_result = "undefined";
	}

	void run_analysis() {
		try {
			init();
			calibrate();

			long long time = 0;
			long n = m_optimal_starting_n;
			long delta = max(m_optimal_starting_n / 10, 1);
			auto t1 = std::chrono::high_resolution_clock::now();

			while (m_benchmark.size() < 200) {
				m_benchmark.insert(std::make_pair(m_get_n(), m_eval()));

				if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t1).count() > m_tenSecs) {
					m_status = "max time exceeded";
					break;
				}

				if (get_used_memory_percentage() > 95) {
					m_status = "max memory exceeded";
					break;
				}

				while (m_get_n() < n + delta)
					m_increase_n();
				n = m_get_n();
			}

			eval_benchmark_slopes();
			eval_results();
		}
		catch (std::exception e) {
			std::cout << "\n\n Error: " << e.what() << std::endl << std::endl;
		}
	};

	void print_benchmark() {
		print(m_benchmark);
	}

	void print_slopes() {
		print(m_slopes);
	}

	std::string get_result() {
		return m_result;
	}

	std::string get_status() {
		return m_status;
	}
};

#endif

