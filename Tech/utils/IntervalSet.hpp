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

		void insert_chronological_solid_intervals(std::vector<T> const& start, std::vector<T> const& finish, int n) {
			// TODO
		}

		void insert_maximum_solid_intervals(std::vector<T> const& start, std::vector<T> const& finish, int n) {
			auto insert_pair = [&](T a, T z) {								// O(log(N))
					m_map.insert(std::make_pair(a, true));
					m_map.insert(std::make_pair(z, false));
				};

			for (size_t i = 0; i < n; i++) {								// O(n)
				const T& a = start[i], z = finish[i];
				if (a >= z)
					continue;

				const auto& it = m_map.lower_bound(a);                      // O(log(N))

				// very left or very right
				if (it == m_map.end()) {
					insert_pair(a, z);
					continue;
				}

				const T& x = it->first;
				const auto& next = std::next(it);

				if (it == m_map.begin() && x != a) {
					if (x > z) {
						insert_pair(a, z);
					}
					else if (x == z) {
						m_map.insert(std::make_pair(a, true));
					}
					else if (x < z && (next->first - x) > (z - a)) {
						m_map.erase(it, next);
						insert_pair(a, z);
					}
					continue;
				}
				
				const auto& prev = std::prev(it);

				// left
				if (it->second && x > a) {
					if (x > z) {
						erase(prev);
						insert_pair(a, z);
					}
					else if (x == z) {
						erase(prev);
						m_map.insert(std::make_pair(a, true));
					}
					// overlapping
					else if (x < z && (next->first - x) >(z - a)) {
						erase(it);
						erase(next);
						insert_pair(a, z);
					}
				}
				else if (it->second && x == a) {
					// inside
					if (next->first > z) {
						erase(next);
						m_map.insert(std::make_pair(z, false));
					}
				}
				// right
				else if (!it->second && x > a) {
					// inside
					if (x >= z) {
						erase(prev);
						erase(it);
						insert_pair(a, z);
					}
					// overlapping
					else if (x < z && next != m_map.end() && next->first > z && (x - prev->first) > (z - a)) {
						erase(prev);
						erase(it);
						insert_pair(a, z);
					}
					else if (x < z && next != m_map.end() && next->first == z && (x - prev->first) > (z - a)) {
						erase(prev);
						erase(it);
						m_map.insert(std::make_pair(a, true));
					}
					else if (x < z && next == m_map.end() && (x - prev->first) >(z - a)) {
						erase(prev);
						erase(it);
						insert_pair(a, z);
					}
				}
				else if (!it->second && x == a) {
					if (next != m_map.end() && next->first > z) {
						m_map.erase(x);
						insert_pair(a, z);
					}
					else if (next != m_map.end() && next->first == z) {
						m_map.erase(x);
						m_map.insert(std::make_pair(a, true));
					}
					else if (next != m_map.end() && next->first < z && (std::next(next)->first - next->first) >(z - a)) {
						m_map.erase(it, std::next(std::next(next)));
						insert_pair(a, z);
					}
					else if (next == m_map.end()) {
						m_map.erase(x);
						insert_pair(a, z);
					}
				}
			}
		}

		void erase(const std::map<T, bool>::iterator& it) {
			if (it != m_map.end() && it != m_map.begin() && std::prev(it)->second)
				it->second = false;
			else if (it != m_map.end())
				m_map.erase(it->first);
		}

	public:

		interval_set(interval_set_mode mode) : m_N(0), m_mode(mode), m_is_cache_valid(true) {}

		void insert(T const& start, T const& finish) {
			m_is_cache_valid = false;
			switch (m_mode)
			{
				case interval_set_mode::chronological: return insert_chronological_solid_intervals({ start }, { finish }, 1);
				case interval_set_mode::maximum: return insert_maximum_solid_intervals({ start }, { finish }, 1);
				default: return;
			};
		}

		void insert(std::vector<T> const& start, std::vector<T> const& finish) {
			auto n = start.size();
			if (n != finish.size())
				throw std::invalid_argument("interval set insert: size mismatch.");
			m_is_cache_valid = false;
			switch (m_mode)
			{
				case interval_set_mode::chronological: return insert_chronological_solid_intervals(start, finish, n);
				case interval_set_mode::maximum:  return insert_maximum_solid_intervals(start, finish, n);
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

