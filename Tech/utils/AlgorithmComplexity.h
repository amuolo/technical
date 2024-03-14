#pragma once

#ifndef INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_
#define INCLUDE_UTILS_ALGORITHMCOMPLEXITY_H_

#include "StandardLibs.h"
#include "RandomMachine.h"
#include "Resources.h"

template <typename TParams>
class algorithm_complexity {
private:
	std::function<void()> m_function;
	std::function<void()> m_increase_n;
	std::function<long long()> m_get_n;
	std::function<void()> m_reset;
	std::function<long long()> m_eval;

	std::map<long, long long> m_benchmark;
	std::string m_result;

	short m_type;
	long long m_optimal_starting_n;
	long long m_optimal_num_it;

	void init() {
		m_result = "undefined";

		m_optimal_num_it = 10;
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
		long long time = 0;
		std::chrono::steady_clock::time_point t1, t2;

		while (time < 100) {
			m_increase_n();
			m_optimal_starting_n = m_get_n();

			if (m_optimal_num_it < 10) m_optimal_num_it++;
			if (m_type < 0) m_increase_n();
			m_eval = [&]() {
				auto t1 = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < m_optimal_num_it; i++) {
					m_function();
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
				};

			time = m_eval();
		}
	};

public:
	algorithm_complexity(
		const std::function<void()>& fun,
		const std::function<long long()> get_n,
		const std::function<void()> reset)
		: m_function(fun), m_increase_n(fun), m_get_n(get_n), m_reset(reset) {
		init();
	};

	algorithm_complexity(
		const std::function<void()>& fun,
		const std::function<void()>& increase_n,
		const std::function<long long()>& get_n,
		const std::function<void()>& reset)
		: m_function(fun), m_increase_n(increase_n), m_get_n(get_n), m_reset(reset) {
		init();
	};

	std::string analysis() {
		try {
			init();
			calibrate();

			long long time = 0;
			long n = m_optimal_starting_n;

			while (time < 1000000 && get_available_memory_percentage() < 95) {
				m_benchmark.insert(std::make_pair(m_get_n(), m_eval()));

				while (m_get_n() < n * 10) {
					m_increase_n();
				}
				n = m_get_n();
			}
		}
		catch (std::exception e) {
			std::cout << "\n\n Error: " << e.what() << std::endl << std::endl;
		}

		return m_result;
	};

	void print_benchmark() {
		print(m_benchmark);
	}
};

#endif

