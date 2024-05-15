#pragma once

#ifndef INCLUDE_UTILS_INTERVALSET_H_
#define INCLUDE_UTILS_INTERVALSET_H_

#include "StandardLibs.hpp"
#include "Algorithms.hpp"

namespace tech
{
	enum interval_set_mode { chronological, maximum };

	template <typename T>
	class interval_set {
		friend static class IntervalSetTest;

	private:
		interval_set_mode m_mode;
		std::map<T, bool> m_map;
		int m_num;

		void assign_chronological(std::vector<T> const& start, std::vector<T> const& finish) {
			// TODO
		}

		void assign_maximum(std::vector<T> const& start, std::vector<T> const& finish) {
			const auto& indices = tech::algorithms::sort_index(finish);     // O(n*log(n))

			auto insert_fun = [&](T a, T z) {								// O(log(N))
					m_map.insert(std::make_pair(a, true));
					m_map.insert(std::make_pair(z, false));
				};

			for (size_t index : indices) {                                  // O(n)
				const auto& a = start[index];
				const auto& z = finish[index];
				if (a >= z)
					continue;

				const auto& it = m_map.lower_bound(a);                      // O(log(N))
				const auto& next = std::next(it);
				const auto& prev = std::prev(it);

				if (it == m_map.end()) {
					insert_fun(a, z);
				}
				else if (it->first == a && !it->second && next->first > z) {
					m_map.erase(a);
					insert_fun(a, z);
				}
				else if (it->first == a && !it->second && it->first == z) {
					m_map.erase(it, next);
				}
				else if (it->first == a && it->second && (next->first - it->first) > (z - a)) {
					m_map.erase(it, next);
					insert_fun(a, z);
				}
				else if (it->first > a && it->second && next->first > z && (next->first - it->first) > (z - a)) {
					m_map.erase(it, next);
					insert_fun(a, z);
				}
				else if (it->first > a && !it->second && next->first > z && (it->first - prev->first) > (z - a)) {
					m_map.erase(prev, it);
					insert_fun(a, z);
				}
				else if (it->first > a && !it->second && next->first > z && (it->first - prev->first) > (z - a)) {
					m_map.erase(it, next);
				}
			}
		}

	public:

		interval_set(interval_set_mode mode) : m_num(0), m_mode(mode) {}

		void assign(T const& start, T const& finish) {
			switch (m_mode)
			{
				case interval_set_mode::chronological: return assign_chronological({ start }, { finish });
				default: return assign_maximum({ start }, { finish });
			};
		}

		void assign(std::vector<T> const& start, std::vector<T> const& finish) {
			if (start.size() != finish.size())
				throw std::invalid_argument("interval set assign: size mismatch.");
			switch (m_mode)
			{
				case interval_set_mode::chronological: return assign_chronological(start, finish);
				default: return assign_maximum(start, finish);
			};
		}

		int count() const {
			return m_num;
		}

		bool IsIn(const T& key) const {
			if (m_map.contains(key))                        // O(log(N))
				return true;
			const auto& it = m_map.lower_bound(key);        // O(log(N))
			if (it == m_map.end())
				return false;
			else if (it == m_map.begin())
				return false;
			else if (std::prev(it)->second == 'a')
				return true;
			else if (std::prev(it)->second == 'z')
				return true;
			else
				return false;
		}
	};

}

#endif

