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
		std::map<T, char> m_map;
		int m_num;

		void assign_chronological(T const& start, T const& finish) {
			// TODO
		}

		void assign_maximum(T const& start, T const& finish) {
			// TODO
		}

		void assign_chronological(std::vector<T> const& start, std::vector<T> const& finish) {
			// TODO
		}

		void assign_maximum(std::vector<T> const& start, std::vector<T> const& finish) {
			const auto& indices = tech::algorithms::sort_index(finish);     // O(n*log(n))
			const int n = indices.size();
			int current_end_time;
			bool begin = true;
			for (size_t i = 0; i < n; i++) {                                // O(n)
				const auto index = indices[i];	                            // O(1)
				const auto& a = start[index];                               // O(1) 
				const auto& z = finish[index];                              // O(1)
				if (begin || a > current_end_time) {
					m_num++;
					begin = false;
					current_end_time = z;
					if (a == z) {
						m_map.insert(std::make_pair(a, 'm'));               // O(log(n))
					}
					else {
						m_map.insert(std::make_pair(a, 'a'));               // O(log(n))
						m_map.insert(std::make_pair(z, 'z'));               // O(log(n))
					}
				}
			}
		}

	public:

		interval_set(interval_set_mode mode) : m_num(0), m_mode(mode) {}

		void assign(T const& start, T const& finish) {
			switch (m_mode)
			{
				case interval_set_mode::chronological: return assign_chronological(start, finish);
				default: return assign_maximum(start, finish);
			};
		}

		void assign(std::vector<T> const& start, std::vector<T> const& finish) {
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
			if (m_map.contains(key)) 
				return true;
			const auto& it = m_map.lower_bound(key);        // O(n)
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

