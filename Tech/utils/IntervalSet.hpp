#pragma once

#ifndef INCLUDE_UTILS_INTERVALSET_H_
#define INCLUDE_UTILS_INTERVALSET_H_

#include "StandardLibs.hpp"
#include "Algorithms.hpp"

namespace tech
{
	/**
	 * interval_set<T> is a data structure
	 */

	enum interval_set_mode { chronological, maximum };

	template <typename T>
	class interval_set {
		friend static class IntervalSetTest;

	private:
		interval_set_mode m_mode;
		std::map<T, bool> m_map;
		int m_N;
		bool m_is_cache_valid;

		void insert_chronological_solid_intervals(std::vector<T> const& start, std::vector<T> const& finish) {
			// TODO
		}

		void insert_maximum_solid_intervals(std::vector<T> const& start, std::vector<T> const& finish) {
			const auto& indices = tech::algorithms::sort_index(finish);     // O(n*log(n))

			auto insert_pair = [&](T a, T z) {								// O(log(N))
					m_map.insert(std::make_pair(a, true));
					m_map.insert(std::make_pair(z, false));
				};

			for (size_t index : indices) {                                  // O(n)
				const auto& a = start[index];
				const auto& z = finish[index];
				if (a >= z)
					continue;

				const auto& it = m_map.lower_bound(a);                      // O(log(N))

				// very left or very right
				if (it == m_map.end() || (it == m_map.begin() && it->first > z)) {
					insert_pair(a, z);
					continue;
				}
				else if (it == m_map.begin() && it->first == z) {
					m_map.insert(std::make_pair(a, true));
					continue;
				}
				else if (it == m_map.begin() && it->first < z && (std::next(it)->first - it->first) > (z - a)) {
					m_map.erase(it, std::next(it));
					insert_pair(a, z);
					continue;
				}

				const auto& next = std::next(it);
				const auto& prev = std::prev(it);

				// left
				if (it->second) {
					if (it->first > z) {
						insert_pair(a, z);
					}
					else if (it->first == z) {
						m_map.insert(std::make_pair(a, true));
					}
					// overlapping
					else if (a < it->first < z && (next->first - it->first) > (z - a)) {
						m_map.erase(it, next);
						insert_pair(a, z);
					}
					// inside
					else if (it->first == a && next->first > z) {
						m_map.erase(next);
						m_map.insert(std::make_pair(z, false));
					}
				}
				// right
				else if (!it->second) {
					// inside
					if (it->first >= z) {
						m_map.erase(prev, it);
						insert_pair(a, z);
					}
					// overlapping
					else if (a < it->first < z && next->first > z && (it->first - prev->first) > (z - a)) {
						m_map.erase(prev, it);
						insert_pair(a, z);
					}
					else if (a < it->first < z && next->first == z && (it->first - prev->first) > (z - a)) {
						m_map.erase(prev, it);
						m_map.insert(std::make_pair(a, true));
					}
				}
			}
		}

	public:

		interval_set(interval_set_mode mode) : m_N(0), m_mode(mode), m_is_cache_valid(true) {}

		void insert(T const& start, T const& finish) {
			m_is_cache_valid = false;
			switch (m_mode)
			{
				case interval_set_mode::chronological: return insert_chronological_solid_intervals({ start }, { finish });
				case interval_set_mode::maximum: return insert_maximum_solid_intervals({ start }, { finish });
				default: return;
			};
		}

		void insert(std::vector<T> const& start, std::vector<T> const& finish) {
			if (start.size() != finish.size())
				throw std::invalid_argument("interval set insert: size mismatch.");
			m_is_cache_valid = false;
			switch (m_mode)
			{
				case interval_set_mode::chronological: return insert_chronological_solid_intervals(start, finish);
				case interval_set_mode::maximum:  return insert_maximum_solid_intervals(start, finish);
				default: return;
			};
		}

		int count() {
			if (m_is_cache_valid)
				return m_N;
			m_N = std::ranges::count_if(m_map, [](const auto& pair){ return pair.second == true; });
			m_is_cache_valid = true;
			return m_N;
		}

		bool contains(const T& key) const {
			const auto& it = m_map.lower_bound(key);		           // O(log(N))

			if (it == m_map.end() || it == m_map.begin() && key < it->first)
				return false;
			else if (it->first == key && it->second)
				return true;
			else if (it != m_map.begin() && it->first > key && std::prev(it)->second)
				return true;
			
			return false;
		}
	};

}

#endif

