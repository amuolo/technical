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
	 * 
	 * 	The following type constraints must be met:
	 *		Key type K
	 *		copyable, assignable, subtractable, and less-than comparable via operators < and <=
	 *		Value type V
	 *		copyable, assignable, and equality-comparable via operator ==
	 */

	enum insert_mode { conservative, progressive, maximum, fluid };

	template <typename K, typename V>
	class interval_mapx {
		friend static class IntervalMapxTest;

	private:
		
		struct feature {
			bool start;
			V value;
		};

		std::map<K, feature> m_map;
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
				const auto result = m_map.insert(std::make_pair(a, feature(true, val)));
				if (not result.second) {
					m_map.erase(a);
					m_map.insert(std::make_pair(a, feature(true, val)));
				}
				m_map.insert(std::make_pair(b, feature(false, val)));
			};

			const auto inner_erase = [&](const std::map<K, feature>::iterator& it) {
				if (it != m_map.end() && it != m_map.begin() && std::prev(it)->second.start)
					it->second.start = false;
				else if (it != m_map.end())
					m_map.erase(it->first);
			};

			for (size_t i = 0; i < n; i++) {								// O(n)
				const K& a = start[i], z = finish[i];
				const V& val = vals[i];
				if (z <= a)
					continue;

				const auto& it = m_map.lower_bound(a);                      // O(log(N))

				if (it == m_map.end()) {
					inner_insert(a, z, val);
					continue;
				}

				const auto& x = it->first;
				const auto& next = std::next(it);

				// left
				if (it->second.start) {
					if (z <= x) {  // outside
						if (it != m_map.begin() && std::prev(it)->second.start)
							inner_erase(std::prev(it));
						inner_insert(a, z, val);
					}
					else if (x < z && (z - a) < (next->first - x)) {  // overlapping
						inner_erase(it);
						inner_erase(next);
						inner_insert(a, z, val);
					}
				}
				// right
				else if (!it->second.start && a < x) {
					const auto& prev = std::prev(it);
					if ((z <= x) ||  // inside
						(x < z && next != m_map.end() && z <= next->first && (z - a) < (x - prev->first)) ||  // overlapping
						(x < z && next == m_map.end() && (z - a) < (x - prev->first))) {
						inner_erase(prev);
						inner_erase(it);
						inner_insert(a, z, val);
					}
				}
				else if (!it->second.start && m_map.find(a) != m_map.end()) {
					if ((next == m_map.end()) ||  // outside
						(next != m_map.end() && z <= next->first)) { // overlapping
						inner_insert(a, z, val);
					}
					else if (next != m_map.end() && next->first < z && (z - a ) < (std::next(next)->first - next->first)) {  // overlapping
						inner_erase(std::next(next));
						inner_erase(next);
						inner_insert(a, z, val);
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
			m_N = std::ranges::count_if(m_map, [](const auto& pair) { return pair.second.start == true; });
			m_is_cache_valid = true;
			return m_N;
		}

		bool contains(const K& key) const {
			const auto& it = m_map.lower_bound(key);			// O(log(N))
			const bool exist = m_map.find(key) != m_map.end();  // O(log(N))

			if (it == m_map.end() || it == m_map.begin() && key < it->first)
				return false;
			else if (exist && it->second.start)
				return true;
			else if (it != m_map.begin() && key < it->first && std::prev(it)->second.start)
				return true;

			return false;
		}
	};
}

#endif
