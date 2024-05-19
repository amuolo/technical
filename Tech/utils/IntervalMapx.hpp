#pragma once

#ifndef INCLUDE_UTILS_INTERVALMAPX_H_
#define INCLUDE_UTILS_INTERVALMAPX_H_

#include "StandardLibs.hpp"
#include "Algorithms.hpp"

namespace tech
{
	/**
	 * interval_set<T> is a data structure that allows to store non-overlapping intervals of type T.
	 * The type T does not need to be default-constructible but must implement the comparison operator <.
	 *
	 * The class can operate in four different modes:
	 *  - conservative: new intervals cannot replace old ones if there is an overlap
	 *  - progressive: new intervals always replace old ones for which there is an overlap, i.e. the newest win
	 *  - maximum: novel intervals can replace old ones if this leads to an increase of the maximum possible number of intervals
	 *  - fluid: new intervals are inserted by modifying the borders of previously inserted intervals
	 */

	enum insert_mode { conservative, progressive, maximum, fluid };

	template <typename K, typename V>
	class interval_mapx {
		friend static class IntervalMapxTest;

	private:
		
		std::map<K, V> m_map;
		V m_valBegin;
		int m_N;

	protected:

		insert_mode m_mode;
		bool m_is_cache_valid;

		void insert_conservative_solid_intervals(std::vector<K> const& start, std::vector<K> const& finish, std::vector<V> const& vals, int n) {
			// TODO
		}

		void insert_progressive_solid_intervals(std::vector<K> const& start, std::vector<K> const& finish, std::vector<V> const& vals, int n) {
			// TODO
		}

		void insert_fluid_solid_intervals(std::vector<K> const& start, std::vector<K> const& finish, std::vector<V> const& vals, int n) {
			// TODO
		}

		void insert_maximum_solid_intervals(std::vector<K> const& start, std::vector<K> const& finish, std::vector<V> const& vals, int n) {
			const auto inner_insert = [&](const K& a, const K& b, const V& val) {
				const auto result = m_map.insert(std::make_pair(a, true));
				if (not result.second) {
					m_map.erase(a);
					m_map.insert(std::make_pair(a, true));
				}
				m_map.insert(std::make_pair(b, false));
				};

			const auto inner_erase = [&](const std::map<K, V>::iterator& it) {
				if (it != m_map.end() && it != m_map.begin() && std::prev(it)->second)
					it->second = false;
				else if (it != m_map.end())
					m_map.erase(it->first);
				};

			for (size_t i = 0; i < n; i++) {								// O(n)
				const K& a = start[i], z = finish[i];
				const V& val = vals[i];
				if (a >= z)
					continue;

				const auto& it = m_map.lower_bound(a);                      // O(log(N))

				if (it == m_map.end()) {
					inner_insert(a, z);
					continue;
				}

				const auto& x = it->first;
				const auto& next = std::next(it);

				// left
				if (it->second) {
					if (x >= z) {  // outside
						if (it != m_map.begin() && std::prev(it)->second)
							inner_erase(std::prev(it));
						inner_insert(a, z);
					}
					else if (x < z && (next->first - x) >(z - a)) {  // overlapping
						inner_erase(it);
						inner_erase(next);
						inner_insert(a, z);
					}
				}
				// right
				else if (!it->second && x > a) {
					const auto& prev = std::prev(it);
					if ((x >= z) ||  // inside
						(x < z && next != m_map.end() && next->first >= z && (x - prev->first) >(z - a)) ||  // overlapping
						(x < z && next == m_map.end() && (x - prev->first) >(z - a))) {
						inner_erase(prev);
						inner_erase(it);
						inner_insert(a, z);
					}
				}
				else if (!it->second && x == a) {
					if ((next == m_map.end()) ||  // outside
						(next != m_map.end() && next->first >= z)) { // overlapping
						inner_insert(a, z);
					}
					else if (next != m_map.end() && next->first < z && (std::next(next)->first - next->first) >(z - a)) {  // overlapping
						inner_erase(std::next(next));
						inner_erase(next);
						inner_insert(a, z);
					}
				}
			}
		}

	public:

		interval_mapx(insert_mode mode, V val) : m_N(0), m_mode(mode), m_valBegin(val), m_is_cache_valid(true) {}

		void insert(K const& start, K const& finish, V const& val) {
			m_is_cache_valid = false;
			switch (m_mode)
			{
			case insert_mode::conservative: return insert_conservative_solid_intervals({ start }, { finish }, { val }, 1);
			case insert_mode::progressive: return insert_progressive_solid_intervals({ start }, { finish }, { val }, 1);
			case insert_mode::maximum: return insert_maximum_solid_intervals({ start }, { finish }, { val }, 1);
			case insert_mode::fluid: return insert_fluid_solid_intervals({ start }, { finish }, { val }, 1);
			default: return;
			};
		}

		void insert(std::vector<K> const& start, std::vector<K> const& finish, std::vector<V> const& vals) {
			auto n = start.size();
			if (n != finish.size())
				throw std::invalid_argument("interval set insert: size mismatch.");
			m_is_cache_valid = false;
			switch (m_mode)
			{
			case insert_mode::conservative: return insert_conservative_solid_intervals(start, finish, vals, n);
			case insert_mode::progressive: return insert_progressive_solid_intervals(start, finish, vals, n);
			case insert_mode::maximum:  return insert_maximum_solid_intervals(start, finish, vals, n);
			case insert_mode::fluid:  return insert_fluid_solid_intervals(start, finish, vals, n);
			default: return;
			};
		}

		int count() {
			if (m_is_cache_valid)
				return m_N;
			m_N = std::ranges::count_if(m_map, [](const auto& pair) { return pair.second == true; });
			m_is_cache_valid = true;
			return m_N;
		}

		bool contains(const K& key) const {
			const auto& it = m_map.lower_bound(key);   // O(log(N))

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

