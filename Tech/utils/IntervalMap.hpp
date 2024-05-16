#pragma once

#ifndef INCLUDE_UTILS_INTERVALMAP_H_
#define INCLUDE_UTILS_INTERVALMAP_H_

#include "StandardLibs.hpp"

namespace tech
{
	/**
	 * interval_map<K,V> is a data structure that associates keys of type K with values of type V.
	 * It is designed to be used efficiently in situations where intervals of consecutive keys are
	 * associated with the same value.
	 *
	 * Each key value pair (k,v) in m_map means that the value v is associated with all keys
	 * from k (including) to the next key (excluding) in m_map. The m_valBegin holds the value that
	 * is associated with all keys less than the first key in m_map.
	 *
	 * The representation is canonical, i.e. consecutive map entries do not contain the same value.
	 * Moreover, the following type constraints are met:
	 *		Key type K
	 *		copyable, assignable, and less-than comparable via operator<
	 *		Value type V
	 *		copyable, assignable, and equality-comparable via operator==
	 *
	 * Note: neither the key nor the value types need to be default-constructible.
	 *
	 * Example: m_valBegin = 'A', m_map = { (1,'B'), (3, 'A') }
	 *		...
	 *		-1 -> 'A'
	 *		 0 -> 'A'
	 *		 1 -> 'B'
	 *		 2 -> 'B'
	 *		 3 -> 'A'
	 *		...
	 */

	template <typename K, typename V>
	class interval_map {
		friend static class IntervalMapTest;

	private:
		V m_valBegin;
		std::map<K, V> m_map;

	public:

		interval_map(V const& val) : m_valBegin(val) {}

		void assign(K const& keyBegin, K const& keyEnd, V const& val) {
			if (!(keyBegin < keyEnd))
				return;

			auto startIt = m_map.lower_bound(keyBegin);				// O(log(n))
			auto finishIt = m_map.lower_bound(keyEnd);				// O(log(n))
			bool isFinishIn = m_map.find(keyEnd) != m_map.end();	// O(log(n))

			V preBeginVal = startIt == m_map.begin() ? m_valBegin : std::prev(startIt)->second;
			V preEndVal = preBeginVal;

			auto it = startIt;

			while (it != finishIt) {								// O(distance(start, finish))
				auto nextIt = std::next(it);
				if (nextIt == finishIt) {
					if (isFinishIn)
						preEndVal = nextIt->second;
					else
						preEndVal = it->second;						// construction & assignments
				}
				it = m_map.erase(it);								// amortized O(1)
			}

			bool isBeginOk = preBeginVal != val;
			bool isFinishOk = (isBeginOk || preBeginVal != preEndVal) && val != preEndVal;

			if (isBeginOk)
				m_map.insert(std::make_pair(keyBegin, val));		// O(log(n))
			if (isFinishOk)
				m_map.insert(std::make_pair(keyEnd, preEndVal));	// O(log(n))
		}

		V const& operator[](K const& key) const {
			auto it = m_map.upper_bound(key);
			if (it == m_map.begin()) {
				return m_valBegin;
			}
			else {
				return (--it)->second;
			}
		}
	};

}

#endif

